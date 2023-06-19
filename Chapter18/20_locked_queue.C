#if __cplusplus < 201703L // C++17 needed
#error "This example requires C++17"
#else
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <optional>

template <typename T> class ts_queue {
    std::queue<T> q_;
    std::mutex m_;
    public:
    ts_queue() = default;
    template <typename U> void push(U&& u) {
        std::lock_guard l(m_);
        q_.push(std::forward<U>(u));
    }
    std::optional<T> pop() {
        std::lock_guard l(m_);
        if (q_.empty()) return {};
        std::optional<T> res(std::move(q_.front()));
        q_.pop();
        return res;
    }
};

ts_queue<int> q;

void produce() {
    q.push(rand());
}

std::mutex m;

void consume() {
    std::optional<int> i = q.pop();
    if (i) {
        std::lock_guard l(m);
        std::cout << "Got " << *i << " from the queue" << std::endl;
    }
}

int main() {
    // Launch producers.
    constexpr size_t npthread = 5;
    std::thread pt[npthread];
    for (size_t i = 0; i != npthread; ++i) {
        pt[i] = std::thread(produce);
    }
    // Launch consumers.
    constexpr size_t ncthread = 50;
    std::thread ct[ncthread];
    for (size_t i = 0; i != ncthread; ++i) {
        ct[i] = std::thread(consume);
    }
    // Join threads.
    for (size_t i = 0; i != ncthread; ++i) {
        ct[i].join();
    }
    for (size_t i = 0; i != npthread; ++i) {
        pt[i].join();
    }
}
#endif // __cplusplus < 201703L // C++17 needed 
