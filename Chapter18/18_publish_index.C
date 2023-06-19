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

std::atomic<size_t> size(0);

void produce() {
    for (size_t n = 0; n != N; ++n) {
        const int niter = 1 << (6 + (rand() % 12));
        data[n].n = niter;
        double& x = data[n].x;
        x = 0;
        // Compute pi using niter iterations.
        for (int i = 0, d = 1; i != niter; ++i, d += 4) {
            x += (4.0/d - 4.0/(d + 2));
        }
        size.store(n, std::memory_order_release);
    }
}

void consume() {
    size_t n = 0;
    do {
        n = size.load(std::memory_order_acquire);
        if (n == 0) continue;
        std::cout << "Consumer sees " << n << " elements produced" << std::endl;
    } while (n < N - 1);
    // Print all data we have produced.
    for (size_t i = 0; i != n; ++i) {
        std::cout << data[i].x << "(" << data[i].n << ") ";
    }
    std::cout << std::endl;
}

int main() {
    // Launch consumer and have it wait.
    std::thread t1(consume);
    // Launch producer.
    std::thread t2(produce);
    // Wait for consumer to finish.
    t1.join();
    // Join producer (it must be done already).
    t2.join();
}
