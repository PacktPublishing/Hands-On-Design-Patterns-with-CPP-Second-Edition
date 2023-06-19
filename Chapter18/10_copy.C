#if __cplusplus < 202002L
#error "This example needs C++20"
#else
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>

void count_unique(std::vector<int> data, size_t& count) {
    std::sort(data.begin(), data.end());
    count = std::unique(data.begin(), data.end()) - data.begin();
}

void count_unique_even(std::vector<int> data, size_t& count) {
    std::erase_if(data, [](int i) { return i & 1; });
    std::sort(data.begin(), data.end());
    count = std::unique(data.begin(), data.end()) - data.begin();
}

int main() {
    std::vector<int> data { 26, 44, 74, 89, 1, 30, 85, 17, 86, 21, 55, 95, 56, 60, 82, 69, 80, 88, 79, 62, 20, 37, 57, 24, 46, 35, 29, 6, 50, 59, 11, 42, 9, 68, 71, 81, 31, 90, 8, 87, 7, 77, 16, 34, 73, 83, 5, 54, 91, 43, 92, 25, 94, 22, 53, 65, 15, 52, 70, 36, 19, 39, 32, 76, 23, 93, 4, 72, 61, 64, 10, 63, 97, 84, 13, 47, 49, 18, 2, 48, 27, 12, 45, 14, 75, 66, 33, 96, 58, 99, 51, 100, 28, 3, 41, 78, 38, 40, 67, 98 };
    size_t unique_count = 0;
    size_t unique_even_count = 0;
    {
    std::jthread t1(count_unique, data, std::ref(unique_count));
    std::jthread t2(count_unique_even, data, std::ref(unique_even_count));
    }
    std::cout << unique_count << " " << unique_even_count << std::endl;
}
#endif // __cplusplus < 202002L
