#if __cplusplus < 202002L
#error "This example needs C++20"
#else
#include <coroutine>
#include <iostream>
#include <chrono>

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
    operator bool() const { return !h_.done(); }
    T operator()() {
        h_.resume();
        return h_.promise().value_;
    }
};

generator<size_t> coro(size_t*** a, size_t N1, size_t N2, size_t N3) {
    for (size_t i = 0; i < N1; ++i) {
        for (size_t j = 0; j < N2; ++j) {
            for (size_t k = 0; k < N3; ++k) {
                //std::cout << "i=" << i << " j=" << j << " k=" << k << " a=" << a[i][j][k] << std::endl;
                co_yield a[i][j][k];
            }
        }
    }
    co_return;
}

class Iterator {
    const size_t N1, N2, N3;
    size_t*** const a;
    size_t i = 0, j = 0, k = 0;
    bool done = false;
    public:
    Iterator(size_t*** a, size_t N1, size_t N2, size_t N3) : N1(N1), N2(N2), N3(N3), a(a) {}
    bool next(size_t& x) {
        if (done) return false;
        x = a[i][j][k];
        //std::cout << "i=" << i << " j=" << j << " k=" << k << " a=" << x << std::endl;
        if (++k == N3) {
            k = 0;
            if (++j == N2) {
                j = 0;
                if (++i == N1) return (done = true);
            }
        }
        return true;
    }
};

int main() {
    constexpr size_t N1 = 1000, N2 = 200, N3 = 150;

    // Allocate the array.
    size_t*** a = new size_t**[N1];
    size_t x = 0;
    for (size_t i = 0; i < N1; ++i) {
        a[i] = new size_t*[N2];
        for (size_t j = 0; j < N2; ++j) {
            a[i][j] = new size_t[N3];
            for (size_t k = 0; k < N3; ++k) {
                //std::cout << "Init: i=" << i << " j=" << j << " k=" << k << " a=" << x << std::endl;
                a[i][j][k] = x++;
            }
        }
    }

    // Iterator.
    {
        auto start = std::chrono::system_clock::now();
        Iterator it(a, N1, N2, N3);
        x = 0;
        size_t i;
        while (it.next(i)) {
            if (i != x++) { std::cout << "Expected " << x - 1 << ", got " << i << std::endl; abort(); }
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Iterator time: " << diff.count() << " s (" << diff.count()/N1/N2/N3 << " s/iteration)" << std::endl;
    }

    // Generator.
    {
        auto start = std::chrono::system_clock::now();
        auto gen = coro(a, N1, N2, N3);

        x = 0;
        while (true) {
            const size_t val = gen();
            if (!gen) break;
            if (val != x++) { std::cout << "Expected " << x - 1 << ", got " << val << std::endl; abort(); }
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Generator time: " << diff.count() << " s (" << diff.count()/N1/N2/N3 << " s/iteration)" << std::endl;
    }

    // Deallocate the array.
    for (size_t i = 0; i < N1; ++i) {
        for (size_t j = 0; j < N2; ++j) {
            delete [] a[i][j];
        }
        delete [] a[i];
    }
    delete [] a;

}
#endif // __cplusplus < 202002L 
