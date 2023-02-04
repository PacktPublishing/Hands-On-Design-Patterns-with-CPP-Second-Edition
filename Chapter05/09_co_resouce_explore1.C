#include <cstddef>
#include <cassert>
#include <coroutine>
#include <iostream>
#include <memory>
#include <utility>
#include <string_view>

template <typename T> struct generator {
    struct promise_type {
        T value_ = -1;

        generator get_return_object() {
            using handle_type = std::coroutine_handle<promise_type>;
            return generator{handle_type::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        std::suspend_always yield_value(T value) {
            value_ = value;
            return {};
        }
        void return_void() {}
    };

    std::coroutine_handle<promise_type> h_;
};


unsigned long advance(unsigned long i) {
    constexpr unsigned long m = 1234567890, k = 987654321;
    i = (i + k) % m;
    return i;
}

generator<unsigned long> coro() {
    unsigned long i = 0;
    while (true) {
        i = advance(i);
        co_yield (i);       // co yield i => co_await promise.yield_value(i)
    }
}

int main1() {
    auto h = coro().h_;
    unsigned long v = 0;
    auto& promise = h.promise();
    for (std::size_t i = 0; i != 6; ++i) {
        std::cout << promise.value_ << " " << (v = advance(v)) << std::endl;
        h();
    }
    h.destroy();
    return 0;
}

template <typename T> class co_resource {
    public:
    struct promise_type;

    private:
    using handle_type = std::coroutine_handle<promise_type>;

    public:
    // Impilcit convert from the intermediate object.
    co_resource(handle_type coro) : coro_(coro) {}//std::cout << "CR(CORO)" << std::endl;}

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
        coro_.resume();         // Resume the coroutine from the co_yield point
        assert(coro_.done());   // Assert that the coroutine ylelds only once
        coro_.destroy();        // Clean up
    }
    
    struct promise_type {
        const T* yielded_value_p = nullptr;

        std::suspend_never initial_suspend() noexcept { /*std::cout << "IS" << std::endl;*/ return {}; }
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
            //std::cout << "GRO" << std::endl;
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

co_resource<my_mutex> make_guard(my_mutex& m) {
    std::cout << "Before locking" << std::endl;
    m.lock();
    co_yield m;
    m.unlock();
    std::cout << "After locking" << std::endl;
}

#include <setjmp.h>

jmp_buf bufferA, bufferB;
double x;

void routineB()
{
    std::cout << "B1" << std::endl;

    {
        uintptr_t r = setjmp(bufferB);
        x = 1234;
        std::cout << "B2 " << r << " " << x << std::endl;
        if (r == 0) longjmp(bufferA, 1);
        else { std::cout << "B2a " << r << " " << x << std::endl; }
    }

    {
        uintptr_t r = setjmp(bufferB);
        x = 3.14;
        std::cout << "B3 " << r << std::endl;
        if (r == 0) longjmp(bufferA, 3);
        else { std::cout << "B3a " << r << " " << x << std::endl; }
    }

    std::cout << "!!!!!!!!!!!!!!!!!!!! BB" << std::endl;
}

void routineA()
{
    std::cout << "A1" << std::endl;

    {
        uintptr_t r = setjmp(bufferA);
        std::cout << "A2 " << r << std::endl;
        if (r == 0) routineB();
        else { std::cout << "A2a " << r << " " << x << std::endl; }
    }

    {
        uintptr_t r = setjmp(bufferA);
        std::cout << "A3 " << r << std::endl;
        if (r == 0) longjmp(bufferB, 2);
        else { std::cout << "A3a " << r << " " << x << std::endl; }
    }

    std::cout << "+++++++++++++++++++ AA " << std::endl;
}

int main() {
#if 0
    my_mutex m;
    try {
    //co_resource<std::string> s { make_string("Boo") };
    co_resource<my_mutex> l { make_guard(m) };
    //throw 0;
    //std::cout << *s << std::endl;
    std::cout << "Work" << std::endl;
    } catch (...) {}
#endif

    routineA();

}

