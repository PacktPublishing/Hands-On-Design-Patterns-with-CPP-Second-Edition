#include <iostream>
#include <typeinfo>

#if not defined(_MSC_VER)
#include <cxxabi.h>
#endif

template <typename T> auto type(T&& p) {
#if not defined(_MSC_VER)
    int status;
    char* name = abi::__cxa_demangle(typeid(p).name(), 0, 0, &status);
    std::string s(name);
    ::free(name);
    return s;
#else
    return std::string(typeid(p).name());
#endif
}

class A {
    public:
    A() { std::cout << "A::A(): " << type(*this) << std::endl; }
    virtual ~A() { std::cout << "A::~A(): " << type(*this) << std::endl; }
    void f() const { std::cout << "A::f: " << type(*this) << std::endl; }
    virtual void g() const { std::cout << "A::g: " << type(*this) << std::endl; }
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
    c.f();
    c.g();
}
