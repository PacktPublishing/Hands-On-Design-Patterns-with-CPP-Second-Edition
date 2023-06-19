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

MutexGuarded<int> i_guarded(0);

void add() {
    i_guarded([](int& i) { ++i; });
}

int main() {
    std::thread t1(add);
    std::thread t2(add);
    t1.join();
    t2.join();
    i_guarded([](int i) { std::cout << i << std::endl; });
}
