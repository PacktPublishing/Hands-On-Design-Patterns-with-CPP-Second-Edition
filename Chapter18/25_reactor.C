#include <atomic>
#include <thread>
#include <iostream>
#include <stdlib.h>

struct Data {
    int n;
    double x;
};

class Reactor {
    static constexpr size_t N = 1024;
    Data data_[N] {};
    std::atomic<size_t> size_{0};
    public:
    bool operator()(size_t n) {
        const size_t s = size_.fetch_add(1, std::memory_order_acq_rel);
        if (s >= N) return false;
        data_[s].n = n;
        double& x = data_[s].x;
        x = 0;
        // Compute pi using n iterations.
        for (size_t i = 0, d = 1; i != n; ++i, d += 4) {
            x += (4.0/d - 4.0/(d + 2));
        }
        return true;
    }
    void print() const {
        const size_t s = size_.load(std::memory_order_relaxed);
        for (size_t i = 0; i != s; ++i) {
            std::cout << data_[i].x << "(" << data_[i].n << ") ";
        }
        std::cout << std::endl;
    }
};

int main() {
    Reactor r;

    // Launch threads.
    constexpr size_t nthread = 10;
    std::thread t[nthread];
    for (size_t i = 0; i != nthread; ++i) {
        t[i] = std::thread([i, &r]()
            {
                unsigned int s = i;
                for (size_t i = 0; i != 10; ++i) r((rand_r(&s) % 65536) + 1024);
            }
        );
    }
    // Wait for threads to finish.
    for (size_t i = 0; i != nthread; ++i) {
        t[i].join();
    }
    // Report the work done.
    r.print();
}

