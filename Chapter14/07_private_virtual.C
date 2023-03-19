#include <iostream>

class Base {
    public:
    virtual ~Base() = default;
    void method1() { 
        method2();
        method3();
    }
    virtual void method2() { std::cout << "Base::method2" << std::endl; }
    private:
    virtual void method3() { std::cout << "Base::method3" << std::endl; }
};

class Derived : public Base {
    private:
    void method2() override { std::cout << "Derived::method2" << std::endl; }
    void method3() override { std::cout << "Derived::method3" << std::endl; }
};

int main() {
    Derived* d = new Derived;
    Base* b = d;
    b->method1();
    b->method2();       // OK, calls Derived::method2()
    //d->method2();       // Does not compile
    delete b;
}
