#include <iostream>

template <typename T, typename P> requires( requires(T t, P p) { t = *p; } ) void f(T i, P p) { std::cout << "f(T, T*)" << std::endl; }
template <typename T, typename P> requires( requires(T t, P p) { t.*p; } ) void f(T i, P p) { std::cout << "f(T, T::*)" << std::endl; }

struct A {
    int i;
    int f() { return 0; }
};

int main() {
    A a;
    f(0, &a.i);
    f(a, &A::i);
}
