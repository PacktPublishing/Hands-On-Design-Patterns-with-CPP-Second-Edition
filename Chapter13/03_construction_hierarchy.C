#include <iostream>
#include <typeinfo>
#include <type_traits>

#if not defined(_MSC_VER)
#include <cxxabi.h>
#endif

template <typename T> auto type(T&& p) {
    std::string name;
    using TT = std::remove_reference_t<T>;
    if (std::is_const<TT>::value) name += "const ";
    if (std::is_volatile<TT>::value) name += "volatile ";

#if not defined(_MSC_VER)
    int status;
    char* mangled_name = abi::__cxa_demangle(typeid(p).name(), 0, 0, &status);
    name += mangled_name;
    ::free(mangled_name);
#else
    name += std::string(typeid(p).name());
#endif

#if 0
    if (std::is_lvalue_reference<decltype(p)>::value) name += "&";
    if (std::is_rvalue_reference<decltype(p)>::value) name += "&&";
#endif // 0 
    return name;
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
#if 0
    { C* p = &c; std::cout << type(p) << std::endl; }
    { const C* p = &c; std::cout << type(p) << std::endl; }
    { C const * p = &c; std::cout << type(p) << std::endl; }
    { C* const p = &c; std::cout << type(p) << std::endl; }
    { C& p = c; std::cout << type(p) << std::endl; }
    { const C& p = c; std::cout << type(p) << std::endl; }
    { C const & p = c; std::cout << type(p) << std::endl; }
    { std::cout << type(std::move(c)) << std::endl; }
#endif // 0 
}
