#include <iostream>

template <typename T>
T increment(T x) { return x + 1; }

int main() {
    std::cout << increment(5) << std::endl;
    std::cout << increment(4.2) << std::endl;
    const char s[] = "xyz";
    std::cout << *increment(s) << std::endl;
}
