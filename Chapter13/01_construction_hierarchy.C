#include <iostream>
#include <typeinfo>

template <typename T> auto type(T&& t) { return typeid(t).name(); }

class A {
    public:
    A() { std::cout << "A::A(): " << type(*this) << std::endl; }
    virtual ~A() { std::cout << "A::~A(): " << type(*this) << std::endl; }
};

class B : public A {
    public:
    B() { std::cout << "B::B(): " << type(*this) << std::endl; }
    ~B() { std::cout << "B::~B(): " << type(*this) << std::endl; }
};

class C : public B {
    public:
    C() { std::cout << "C::C(): " << type(*this) << std::endl; }
    ~C() { std::cout << "C::~C(): " << type(*this) << std::endl; }
};

int main() {
    C c;
}
