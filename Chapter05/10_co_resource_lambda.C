#include <cassert>
#include <coroutine>
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>

template <typename T> class co_resource {
  public:
  struct promise_type;

  private:
  using handle_type = std::coroutine_handle<promise_type>;

  public:
  co_resource(handle_type coro) : coro_(coro) {}

  co_resource(const co_resource&) = delete;
  co_resource& operator=(const co_resource&) = delete;
  co_resource(co_resource&& from) : coro_(std::exchange(from.coro_, nullptr)) {}
  co_resource& operator=(co_resource&& from) {
    std::destroy_at(this);
    std__construct_at(this, std::move(from));
    return *this;
  }

  ~co_resource() { 
    if (!coro_) return;
    coro_.resume();          // Resume the coroutine from the co_yield point
    assert(coro_.done());    // Assert that the coroutine ylelds only once
    coro_.destroy();         // Clean up
  }
  
  struct promise_type {
    const T* yielded_value_p = nullptr;

    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() noexcept {}
    void unhandled_exception() { throw; }
    std::suspend_always yield_value(const T& value) noexcept {
      yielded_value_p = &value;
      return {};
    }
    // get_return_object is called before initial_suspend(), so before the
    // pre-construction user code. However, conversion from the return type to
    // co_resource happens after initial_suspend() and after that code. 
    // This allows us to return an intermediate object convertible to co_resource
    // to delay construction of the co_resource object.
    handle_type get_return_object() { return handle_type::from_promise(*this); }
  };

  const T& operator*() const noexcept {
    assert(!coro_.done());   // Assert that the coroutine ylelds
    assert(coro_.promise().yielded_value_p);
    return *coro_.promise().yielded_value_p;
  }

  const T* operator->() const noexcept {
    assert(!coro_.done());   // Assert that the coroutine ylelds
    return coro_.promise().yielded_value_p;
  }

  private:
  handle_type coro_;

};

#include <mutex>
std::mutex m;
const bool mt_run = true;

int main(int argc, char** argv) {
  try {
    auto lg { [&](bool log) ->co_resource<std::mutex> {
      if (log) std::cout << "Before locking" << std::endl;
      if (mt_run) m.lock();
      co_yield m;
      if (mt_run) m.unlock();
      if (log) std::cout << "After locking" << std::endl;
    }(true) };
    if (argc > 1) throw 0;
    std::cout << "Work" << std::endl;
  } catch (...) {}
}

