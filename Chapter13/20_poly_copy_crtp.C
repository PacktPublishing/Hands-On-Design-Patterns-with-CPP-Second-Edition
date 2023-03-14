#include <iostream>
#include <memory>
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

class Base {
    public:
    virtual ~Base() {}
    virtual Base* clone() const = 0;
};

template <typename Derived> class Cloner : public Base {
    public:
    Base* clone() const override {
        return new Derived(*static_cast<const Derived*>(this));
    }
};

class Derived : public Cloner<Derived> {
    public:
};

int main() {
    Base* b0 = new Derived;
    Base* b1 = b0->clone();
    std::cout << type(*b0) << std::endl;
    std::cout << type(*b1) << std::endl;
}
