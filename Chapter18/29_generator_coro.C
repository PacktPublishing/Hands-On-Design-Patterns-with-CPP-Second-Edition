#include <coroutine>
#include <iostream>

template <typename T> struct generator {
  struct promise_type {
    T value_ {};

    generator get_return_object() {
      using handle_type = std::coroutine_handle<promise_type>;
      return generator{handle_type::from_promise(*this)};
    }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { abort(); }
    std::suspend_always yield_value(const T value) {
      value_ = value;
      return {};
    }
    void return_void() {}
  };

  using handle_type = std::coroutine_handle<promise_type>;
  handle_type h_;
  generator(handle_type h): h_(h) {}
  ~generator() {
    if (h_) {
      h_.destroy();
    }
  }
  generator(const generator&) = delete;
  generator& operator=(const generator&) = delete;
  generator(generator&& rhs): h_(rhs.h_) {
    rhs.h_ = nullptr;
  }
  generator& operator=(generator&& rhs) {
    h_ = rhs.h_;
    rhs.h_ = nullptr;
    return *this;
  }
  T operator()() {
    h_.resume();
    return h_.promise().value_;
  }
};

generator<size_t> coro(size_t i) {
  while (true) {
    constexpr size_t m = 1234567890, k = 987654321;
    for (size_t j = 0; j != 11; ++j) {
      if (1) i = (i + k) % m; else ++i;
    }
    co_yield i;
  }
}

int main() {
  auto gen = coro(42);
  for (size_t i = 0; i != 100; ++i) {
    std::cout << gen() << " ";
  }
  std::cout << std::endl;
}
