// Variadic template function (C++17, fold)
#include <iostream>
#include <type_traits>

template <typename ... T> auto sum(const T& ... x) { return (x + ...); }

int main() {
    std::cout << sum(5, 7, 3) << std::endl;
    static_assert(std::is_same_v<decltype(sum(5, 7, 3)), int>);
    std::cout << sum(5, 7L, 3) << std::endl;
    static_assert(std::is_same_v<decltype(sum(5, 7L, 3)), long>);
    std::cout << sum(5, 7L, 2.9) << std::endl;
    static_assert(std::is_same_v<decltype(sum(5, 7L, 2.9)), double>);
}
