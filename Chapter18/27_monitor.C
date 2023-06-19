#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>
#include <stdlib.h>

static constexpr size_t N = 1UL << 16;

struct Data {
    int n;
    double x;
};

Data data[N] {};

std::atomic<size_t> index(0);

void produce(std::atomic<size_t>& count) {
    for (size_t n = 0; ; ++n) {
        const size_t s = index.fetch_add(1, std::memory_order_acq_rel);
        if (s >= N) return;         // Not == - we can overshoot the end!
        const int niter = 1 << (8 + data[s].n);
        double& x = data[s].x;
        x = 0;
        // Compute pi using niter iterations.
        for (int i = 0, d = 1; i != niter; ++i, d += 4) {
            x += (4.0/d - 4.0/(d + 2));
        }
        count.store(n + 1, std::memory_order_relaxed);
    }
}

int main() {
    // Fill the data.
    for (size_t i = 0; i != N; ++i) {
        data[i].n = (rand() % 10) + 1;
    }
    // Launch the monitor.
    constexpr size_t nthread = 5;
    std::atomic<size_t> work_count[nthread] = {};
    std::atomic<bool> done {false};
    std::thread monitor([&]()
        {
            auto print = [&]() {
                size_t total_count = 0;
                for (size_t i = 0; i != nthread; ++i) {
                    const size_t n = work_count[i].load(std::memory_order_relaxed);
                    std::cout << n << " ";
                    total_count += n;
                }
                std::cout << "-> " << total_count << "/" << N << std::endl;
            };
            std::cout << "work counts:" << std::endl;
            while (!done.load(std::memory_order_relaxed)) {
                std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
                print();
            }
            print();
        }
    );
    // Launch producers.
    std::thread t[nthread];
    for (size_t i = 0; i != nthread; ++i) {
        t[i] = std::thread(produce, std::ref(work_count[i]));
    }
    // Wait for producers to finish.
    for (size_t i = 0; i != nthread; ++i) {
        t[i].join();
    }
    // Shut down the monitor.
    done.store(true, std::memory_order_relaxed);
    monitor.join();
}
