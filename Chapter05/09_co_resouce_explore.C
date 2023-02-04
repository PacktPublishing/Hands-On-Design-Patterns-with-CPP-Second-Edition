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
    co_resource(handle_type coro) : coro_(coro) {
        std::cout << "CR(CORO)" << std::endl;
    }

    co_resource(const co_resource&) = delete;
    co_resource& operator=(const co_resource&) = delete;
    co_resource(co_resource&& from) : coro_(std::exchange(from.coro_, nullptr)) {}
    co_resource& operator=(co_resource&& from) {
        std::destroy_at(this);
        std__construct_at(this, std::move(from));
        return *this;
    }

~co_resource() { 
    std::cout << "CR(~CORO 1)" << std::endl;
    if (!coro_) return;
    coro_.resume();     // Resume the coroutine from the co_yield point
    std::cout << "CR(~CORO 2)" << std::endl;
    assert(coro_.done());   // Assert that the coroutine ylelds only once
    coro_.destroy();    // Clean up
    std::cout << "CR(~CORO 3)" << std::endl;
}

struct promise_type {
    const T* yielded_value_p = nullptr;

    std::suspend_never initial_suspend() noexcept {
        std::cout << "IS" << std::endl; 
        return {};
    }
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
    auto get_return_object() {
        std::cout << "GRO" << std::endl;
#if 1
        return handle_type::from_promise(*this);
#else
        return co_resource{handle_type::from_promise(*this)};
#endif
    }
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

#include <string>
#include <string_view>

co_resource<std::string> make_string(std::string_view s) {
    std::cout << "Before" << std::endl;
    co_yield std::string(s);
    std::cout << "After" << std::endl;
}

#include <mutex>

class my_mutex : public std::mutex {
    public:
    void lock() { std::cout << "lock" << std::endl; std::mutex::lock(); }
    void unlock() { std::cout << "unlock" << std::endl; std::mutex::lock(); }
};

co_resource<std::mutex> make_guard(std::mutex& m, bool log) {
    if (log) std::cout << "Before locking" << std::endl;
    m.lock();
    co_yield m;
    m.unlock();
    if (log) std::cout << "After locking" << std::endl;
}

int main(int argc, char** argv) {
    my_mutex m;
    try {
#if 0
        co_resource<std::string> s { make_string("Boo") };
        if (argc > 1) throw 0;
        std::cout << *s << std::endl;
#elif 0
        co_resource<my_mutex> l { make_guard(m) };
        if (argc > 1) throw 0;
        std::cout << "Work" << std::endl;
#else
        co_resource<std::mutex> l { [&](bool log) ->co_resource<std::mutex> {
            if (log) std::cout << "Before locking" << std::endl;
            m.lock();
            co_yield m;
            m.unlock();
            if (log) std::cout << "After locking" << std::endl;
        }(true) };
        if (argc > 1) throw 0;
        std::cout << "Work" << std::endl;
#endif
    } catch (...) {}

}

