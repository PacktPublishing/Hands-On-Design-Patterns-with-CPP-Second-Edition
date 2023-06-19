#if __cplusplus < 201703L // C++17 needed
#error "This example requires C++17"
#else
#include <atomic>
#include <mutex>
#include <functional>
#include <thread>
#include <iostream>
#include <stdlib.h>
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

class Proactor {
    using callback_t = std::function<void(size_t, double)>;
    struct op_task {
        size_t n;
        callback_t f;
    };
    std::atomic<bool> done_{false};             // Must come before t_ to initialize in order
    ts_queue<op_task> q_;                       // Must come before t_ to initialize in order
    std::thread t_;
    public:
    Proactor() : t_([this]()
        {
            while (true) {
                auto task = q_.pop();
                if (!task) {                                            // Queue is empty
                    if (done_.load(std::memory_order_relaxed)) return;  // Work is done
                    continue;                                           // Work is not done, wait for more work
                }
                // Execute the task.
                const size_t n = task->n;
                double x = 0;
                x = 0;
                // Compute pi using n iterations.
                for (size_t i = 0, d = 1; i != n; ++i, d += 4) {
                    x += (4.0/d - 4.0/(d + 2));
                }
                task->f(n, x);
            }
        }
    ) {}
    template <typename F> void operator()(size_t n, F&& f) { q_.push(op_task{n, std::forward<F>(f)}); }
    ~Proactor() {
        done_.store(true, std::memory_order_relaxed);
        t_.join();                                                      // Wait for queue to become empty, then exit thread
    }
};

int main() {
    {
    Proactor p;
    for (size_t i = 0; i != 100; ++i) {
        p((rand() % 65536) + 1024, 
          [](size_t n, double x) { std::cout << x << "(" << n << ") "; });
    }
    }
    std::cout << std::endl;
}
#endif // __cplusplus < 201703L
