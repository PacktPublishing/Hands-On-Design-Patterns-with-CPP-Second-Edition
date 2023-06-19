#if __cplusplus < 202002L // C++20 needed
#error "This example requires C++20"
#else
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
    flag.notify_one(); // Wake up the main thread - work is done
}

int main() {
    flag.clear();
    // Launch producer.
    std::thread t(produce);
    // Wait for the work to be done.
    flag.wait(true, std::memory_order_acquire);
    while (!flag.test(std::memory_order_acquire)) {} // Redundant but needed for TSAN
    std::cout << data.n << " " << data.x << std::endl;
    // Join producer (it must be done already).
    t.join();
}
#endif // __cplusplus < 202002L
