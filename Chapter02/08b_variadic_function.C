// Variadic template function (C++14, recursion)
#include <iostream>
#include <type_traits>

template <typename T1> auto sum(const T1& x1) { return x1; }
template <typename T1, typename ... T> auto sum(const T1& x1, const T& ... x) { return x1 + sum(x ...); }

int main() {
    std::cout << sum(5, 7, 3) << std::endl;
    static_assert(std::is_same<decltype(sum(5, 7, 3)), int>::value, "");
    std::cout << sum(5, 7L, 3) << std::endl;
    static_assert(std::is_same<decltype(sum(5, 7L, 3)), long>::value, "");
    std::cout << sum(5, 7L, 2.9) << std::endl;
    static_assert(std::is_same<decltype(sum(5, 7L, 2.9)), double>::value, "");
}
