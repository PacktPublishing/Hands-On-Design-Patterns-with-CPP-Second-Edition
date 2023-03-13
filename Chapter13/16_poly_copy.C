#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

template <typename T> auto type(T&& p) {
    int status;
    return abi::__cxa_demangle(typeid(p).name(), 0, 0, &status);
}

class Base {
    public:
    virtual ~Base() {}
    virtual Base* clone() const = 0;
};

class Derived : public Base {
    public:
    Derived* clone() const override { return new Derived(*this); }
};

int main() {
    Base* b0 = new Derived;
    Base* b1 = b0->clone();
    std::cout << type(*b0) << std::endl;
    std::cout << type(*b1) << std::endl;
}
