// 01 with a race, detectable by TSAN
#include <mutex>
#include <thread>
#include <iostream>

std::mutex m;
int i = 0;

void add() {
    //std::lock_guard<std::mutex> l(m);
    ++i;
}

int main() {
    std::thread t1(add);
    std::thread t2(add);
    t1.join();
    t2.join();
    std::cout << i << std::endl;
}
