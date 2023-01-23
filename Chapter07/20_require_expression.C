#include <iostream>

template <typename T, typename P> void f(T i, P p) requires( requires { i = *p; } ) { std::cout << "f(T, T*)" << std::endl; }
template <typename T, typename P> void f(T i, P p) requires( requires { i.*p; } ) { std::cout << "f(T, T::*)" << std::endl; }
template <typename T, typename P> requires( requires(T a, P b) { a/b; } ) void f(T i, P p) { std::cout << "f(T, P)" << std::endl; }

struct A {
    int i;
    int f() { return 0; }
};

int main() {
    A a;
    f(0, &a.i);
    f(a, &A::i);
    f(1, 2.0);
}
