#include <atomic>
#include <thread>
#include <iostream>

struct Data {
    int n {};
    double x {};
};

std::atomic_flag flag;
Data data {};

void produce() {
    {
        data.n = 1 << 20;
        data.x = 0;
        // Compute pi using n iterations.
        for (int i = 0, d = 1; i != data.n; ++i, d += 4) {
            data.x += (4.0/d - 4.0/(d + 2));
        }
    }
    flag.test_and_set(std::memory_order_release);
}

int main() {
    flag.clear();
    // Launch producer.
    std::thread t(produce);
    // Wait for the work to be done.
    while (!flag.test(std::memory_order_acquire)) {}
    std::cout << data.n << " " << data.x << std::endl;
    // Join producer (it must be done already).
    t.join();
}
