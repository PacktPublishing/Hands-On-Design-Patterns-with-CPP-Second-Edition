#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <mutex>

static constexpr size_t N = 1024;

struct Data {
    int n;
    double x;
};

std::atomic<Data*> data[N] {};

std::atomic<size_t> size(0);

static constexpr double pi = 3.141592653589793238462643;

std::mutex m;
double min_err_all = 2*pi;              // Guarded by m
std::atomic<size_t> size_consumed(0);   // Guarded by m for writing

void produce(int n, int delay) {
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay % 100)); // Just to make threads run in order other than 0, 1, 2...
    const int niter = 1 << (8 + n);
    Data* p = new Data;
    p->n = niter;
    p->x = 0;
    // Compute pi using niter iterations.
    for (int i = 0, d = 1; i != niter; ++i, d += 4) {
        p->x += (4.0/d - 4.0/(d + 2));
    }
    const size_t s = size.fetch_add(1, std::memory_order_relaxed);
    data[s].store(p, std::memory_order_release);
}

void consume() {
    long min_i = -1;
    double min_err = 2*pi; // Larger than any error
    // Find the most accurate value among currently computed.
    size_t i = 0;
    for (; i != N; ++i) {
        const Data* p = data[i].load(std::memory_order_acquire);
        if (!p) break;
        const double err = std::abs(p->x - pi);
        if (err < min_err) {
            min_i = i;
            min_err = err;
        }
    }
    if (min_i < 0) return;
    std::lock_guard<std::mutex> l(m);
    if (size_consumed.load(std::memory_order_relaxed) < i) size_consumed.store(i, std::memory_order_relaxed);
    if (min_err >= min_err_all) return;
    min_err_all = min_err;
    std::cout << i << " elements produced, data[" << min_i << "] is the most accurate (" << min_err << ")" << std::endl;
}

int main() {
    // Launch producers.
    constexpr size_t npthread = 20;
    std::thread pt[npthread];
    for (size_t i = 0; i != npthread; ++i) {
        pt[i] = std::thread(produce, i, rand());
    }
    // Launch consumers until all data is produced.
    do {
        constexpr size_t ncthread = 5;
        std::thread ct[ncthread];
        for (size_t i = 0; i != ncthread; ++i) {
            ct[i] = std::thread(consume);
        }
        for (size_t i = 0; i != ncthread; ++i) {
            ct[i].join();
        }
    } while (size_consumed.load(std::memory_order_relaxed) < npthread);
    // Wait for threads to finish.
    for (size_t i = 0; i != npthread; ++i) {
        pt[i].join();
    }
    // Print all data we have produced.
    for (size_t i = 0, n = size.load(std::memory_order_relaxed); i != n; ++i) {
        Data* p = data[i].load();
        std::cout << p->x << "(" << p->n << ") ";
        delete p;
    }
    std::cout << std::endl;
}
