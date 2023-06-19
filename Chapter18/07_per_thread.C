#include <thread>
#include <iostream>

void events(unsigned int s, size_t& n) {
    for (size_t i = 1; i != 100; ++i) {
        if ((rand_r(&s) % 10) == 0) ++n;
    }
}

int main() {
    alignas(64) size_t n1 = 0;
    alignas(64) size_t n2 = 0;
    std::thread t1(events, 1, std::ref(n1));
    std::thread t2(events, 2, std::ref(n2));
    t1.join();
    t2.join();
    size_t count = n1 + n2;
    std::cout << count << std::endl;
}
