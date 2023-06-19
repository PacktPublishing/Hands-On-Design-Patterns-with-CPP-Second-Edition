#include <mutex>
#include <thread>
#include <iostream>

template <typename T> class MutexGuarded {
    std::mutex m_;
    T data_ {};
    public:
    MutexGuarded() = default;
    template <typename... Args> explicit MutexGuarded(Args&&... args) : data_(std::forward<Args>(args)...) {}
    template <typename F> decltype(auto) operator()(F f) {
        std::lock_guard<std::mutex> l(m_);
        return f(data_);
    }
};

MutexGuarded<size_t> count;

void events(unsigned int s) {
    for (size_t i = 1; i != 100; ++i) {
        if ((rand_r(&s) % 10) == 0) count([](size_t& i) { ++i; });
    }
}

int main() {
    std::thread t1(events, 1);
    std::thread t2(events, 2);
    t1.join();
    t2.join();
    count([](int i) { std::cout << i << std::endl; });
}
