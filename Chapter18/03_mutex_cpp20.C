// C++20 version of 01
#if __cplusplus < 202002L
#error "This example needs C++20"
#else
#include <mutex>
#include <thread>
#include <iostream>

std::mutex m;
int i = 0;

void add() {
    std::lock_guard l(m);
    ++i;
}

int main() {
    {
    std::jthread t1(add);
    std::jthread t2(add);
    }
    std::cout << i << std::endl;
}
#endif // __cplusplus < 202002L
