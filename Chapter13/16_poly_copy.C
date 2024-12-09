#include <iostream>
#include <typeinfo>

#if not defined(_MSC_VER)
#include <cxxabi.h>
#endif

template <typename T> auto type(T&& p) {
#if not defined(_MSC_VER)
    int status;
    return abi::__cxa_demangle(typeid(p).name(), 0, 0, &status);
#else
return std::string(typeid(p).name());
#endif
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
