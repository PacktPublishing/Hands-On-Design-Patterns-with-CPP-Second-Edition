#include <atomic>
#include <thread>
#include <iostream>

std::atomic<size_t> count;

void events(unsigned int s) {
    for (size_t i = 1; i != 100; ++i) {
        if ((rand_r(&s) % 10) == 0) count.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    std::thread t1(events, 1);
    std::thread t2(events, 2);
    t1.join();
    t2.join();
    std::cout << count.load(std::memory_order_relaxed) << std::endl;
}
