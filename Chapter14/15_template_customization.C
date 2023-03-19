#include <iostream>

void f() { std::cout << "::f()" << std::endl; }

template <typename T> struct A {
    void f() const { std::cout << "A::f()" << std::endl; }
    void g() const { std::cout << "A::g()" << std::endl; }
};

template <typename T> struct B : public A<T> {
    void h1() { f(); }     // ::f() not A::f()
    void h2() { this->f(); }
    void h3() { ::f(); }

    // void h4() { g(); }     // Should not compile (buggy compilers call A::g())
    void h5() { this->g(); }
};

struct A1 {
    void f() const { std::cout << "A1::f()" << std::endl; }
    void g() const { std::cout << "A1::g()" << std::endl; }
};

struct B1 : public A1 {
    void h1() { f(); }
    void h2() { this->f(); }
    void h3() { ::f(); }

    void h4() { g(); }
    void h5() { this->g(); }
};

int main () {
    B<int> b;
    b.h1();
    b.h2();
    b.h3();

    // b.h4(); // Does not compile
    b.h5();

    std::cout << std::endl;

    B1 b1;
    b1.h1();
    b1.h2();
    b1.h3();

    b1.h4();
    b1.h5();
}
