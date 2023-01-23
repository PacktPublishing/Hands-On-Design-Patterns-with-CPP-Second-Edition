#include <iostream>

template <typename T> requires(sizeof(T) == 8) void f(T&&) { std::cout << "f(T), sizeof(T) = " << sizeof(T) << " == 8" << std::endl; }
template <typename T> void f(T&& t) requires(sizeof(t) < 8) { std::cout << "f(T), sizeof(T) = " << sizeof(T) << " < 8" << std::endl; }
template <typename T> void g(T p) requires(sizeof(*p) < 8) { std::cout << "f(T), sizeof(*T) = " << sizeof(*p) << " < 8" << std::endl; }

int main() {
    f(0.0);
    f(0);
    int i;
    double d; (void)d;
    g(&i);
    //g(&d); // Constraint fails
}
