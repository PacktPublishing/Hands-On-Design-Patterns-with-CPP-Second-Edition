#if __cplusplus < 201703L // C++17 needed
#error "This example requires C++17"
#else
#include <atomic>
#include <thread>
#include <iostream>
#include <optional>

template <typename T, size_t N> class ts_queue {
    T buffer_[N];
    std::atomic<size_t> back_{0};
    std::atomic<size_t> front_{N - 1};
    public:
    ts_queue() = default;
    ts_queue(const ts_queue&) = delete;
    ts_queue& operator=(const ts_queue&) = delete;
    template <typename U> bool push(U&& u) {
        const size_t front = front_.load(std::memory_order_acquire);              // First used slot
        size_t back = back_.load(std::memory_order_relaxed);                    // Slot to write into
        if (back == front) return false;                                         // Queue is full
        buffer_[back] = std::forward<U>(u);
        back_.store((back + 1) % N, std::memory_order_release);
        return true;
    }
    std::optional<T> pop() {
        const size_t back = back_.load(std::memory_order_acquire);              // Last written slot
        const size_t front = (front_.load(std::memory_order_relaxed) + 1) % N;    // Next slot to read
        if (front == back) return {};                                            // Queue is empty
        std::optional<T> res(std::move(buffer_[front]));
        front_.store(front, std::memory_order_release);
        return res;
    }
    void dump() const {
        const size_t front = front_.load(std::memory_order_relaxed);
        const size_t back = back_.load(std::memory_order_relaxed);
        std::cout << "back: " << back << " front " << front;
        for (size_t i = (front + 1) % N; i != back; i = (i + 1) % N) {
            std::cout << " " << buffer_[i] << "[" << i << "]";
        }
        std::cout << std::endl;
    }
};

ts_queue<int, 8> q;

int main() {
#if 0
    std::cout << "Before: "; q.dump();
    size_t j = 1;
    for (size_t i = 0; i != 5; ++i, j += 2) {
        std::cout << "Push " << j << " " << q.push(j) << " ";
        q.dump();
    }
    for (size_t i = 0; i != 7; ++i) {
        auto res = q.pop();
        if (res) std::cout << "Pop " << *res << " ";
        else std::cout << "Pop - empty ";
        q.dump();
    }

    for (size_t i = 0; i != 5; ++i, j += 2) {
        std::cout << "Push " << j << " " << q.push(j) << " ";
        q.dump();
    }
    for (size_t i = 0; i != 7; ++i) {
        auto res = q.pop();
        if (res) std::cout << "Pop " << *res << " ";
        else std::cout << "Pop - empty ";
        q.dump();
    }
#endif // 0 
    std::atomic<bool> done(false);
    size_t n_full = 0;
    std::thread producer([&]()
    {
        for (size_t i = 0; i != 150; ++i) {
            while (!q.push(2*i + 1)) { ++n_full; }
            if (i && (i % 10) == 0) std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(1)); // Let the consumer catch up
        }
        done.store(true, std::memory_order_release);
    }
    );
    std::thread consumer([&]()
    {
        size_t n_empty = 0;
        while (1) {
            std::optional<int> res = q.pop();
            if (res) {
                std::cout << *res << " " << std::flush;
            } else {
                if (done.load(std::memory_order_acquire)) break;
                ++n_empty;
            }
        }
        std::cout << std::endl << "Queue was empty " << n_empty << " times" << std::endl;
    }
    );
    producer.join();
    consumer.join();
    std::cout << "Queue was full " << n_full << " times" << std::endl;
}
#endif // __cplusplus < 201703L
