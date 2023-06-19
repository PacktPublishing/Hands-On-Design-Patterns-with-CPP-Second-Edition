#include <atomic>
#include <thread>
#include <iostream>
#include <stdlib.h>

static constexpr size_t N = 1024;

struct Data {
    int n;
    double x;
};

Data data[N] {};

std::atomic<size_t> index(0);

void produce(size_t& n) {
    for (n = 0; ; ++n) {
        const size_t s = index.fetch_add(1, std::memory_order_relaxed);
        if (s >= N) return;         // Not == - we can overshoot the end!
        const int niter = 1 << (8 + data[s].n);
        double& x = data[s].x;
        x = 0;
        // Compute pi using niter iterations.
        for (int i = 0, d = 1; i != niter; ++i, d += 4) {
            x += (4.0/d - 4.0/(d + 2));
        }
    }
}

int main() {
    // Fill the data.
    for (size_t i = 0; i != N; ++i) {
        data[i].n = (rand() % 10) + 1;
    }
    // Launch producers.
    constexpr size_t nthread = 5;
    std::thread t[nthread];
    size_t work_count[nthread] = {};
    for (size_t i = 0; i != nthread; ++i) {
        t[i] = std::thread(produce, std::ref(work_count[i]));
    }
    // Wait for producers to finish.
    for (size_t i = 0; i != nthread; ++i) {
        t[i].join();
    }
    // Report the work done.
    for (size_t i = 0; i != nthread; ++i) {
        std::cout << "Thread " << i << " computed " << work_count[i] << " values" << std::endl;
    }
}
