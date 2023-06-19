#include <thread>
#include <vector>
#include <iostream>

template <typename IT, typename T>
void clamp(IT from, IT to, T value) {
    for (IT it = from; it != to; ++it) {
        if (*it > value) *it = value;
    }
}

int main() {
    std::vector<int> data { 26, 44, 74, 89, 1, 30, 85, 17, 86, 21, 55, 95, 56, 60, 82, 69, 80, 88, 79, 62, 20, 37, 57, 24, 46, 35, 29, 6, 50, 59, 11, 42, 9, 68, 71, 81, 31, 90, 8, 87, 7, 77, 16, 34, 73, 83, 5, 54, 91, 43, 92, 25, 94, 22, 53, 65, 15, 52, 70, 36, 19, 39, 32, 76, 23, 93, 4, 72, 61, 64, 10, 63, 97, 84, 13, 47, 49, 18, 2, 48, 27, 12, 45, 14, 75, 66, 33, 96, 58, 99, 51, 100, 28, 3, 41, 78, 38, 40, 67, 98 };
    std::thread t1([&](){ clamp(data.begin(), data.begin() + data.size()/2, 42); });
    std::thread t2([&](){ clamp(data.begin() + data.size()/2, data.end(), 42); });
    t1.join();
    t2.join();
    for (int i : data) std::cout << i << " ";
    std::cout << std::endl;
}
