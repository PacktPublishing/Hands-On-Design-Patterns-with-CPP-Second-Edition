// Constraint
#include <iostream>

template <typename T> T copy(T&& t)
    requires (sizeof(T) > 1)
{
    return std::forward<T>(t);
}

template <typename T> requires (sizeof(T) > 1) T copy1(T&& t)
{
    return std::forward<T>(t);
}

int main() {
    std::cout << copy(1) << std::endl;
    //std::cout << copy('1') << std::endl;
}
