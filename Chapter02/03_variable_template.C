#include <iostream>

template <typename T> constexpr T pi = T(3.14159265358979323846264338327950288419716939937510582097494459230781L);

int main() {
    std::cout << "3.141592653589793" << std::endl;
    std::cout.precision(7);
    std::cout << pi<float> << std::endl;
    std::cout.precision(16);
    std::cout << pi<double> << std::endl;
}
