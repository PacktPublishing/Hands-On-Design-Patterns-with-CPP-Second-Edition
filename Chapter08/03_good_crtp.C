#include <iostream>

template <typename D> class B {
    public:
    B() : i_(0) {}
    virtual ~B() {}
    void f(int i) { // Equivalent of a virtual with a default
        std::cout << "B::f() calling D::f()..." << std::endl;
        static_cast<D*>(this)->f_impl(i);
        std::cout << "B::f() done!" << std::endl;
    }
    void g(int i) { // Equivalent of a pure virtual
        std::cout << "B::g() calling D::g()..." << std::endl;
        static_cast<D*>(this)->g_impl(i);
        std::cout << "B::g() done!" << std::endl;
    }
    int get() const { return i_; }
    protected:
    void f_impl(int i) {
        std::cout << "Running B::f()..." << std::endl;
        i_ += i;
        std::cout << "B::f() done" << std::endl;
    }
    int i_;
};
class D1 : public B<D1> {
    public:
    void f_impl(int i) {
        std::cout << "Running D1::f()..." << std::endl;
        i_ += i;
        std::cout << "D1::f() done" << std::endl;
    }
    void g_impl(int i) {
        std::cout << "Running D1::g()..." << std::endl;
        i_ += i;
        std::cout << "D1::g() done" << std::endl;
    }
};
class D2 : public B<D2> {
    public:
    // No f_impl() here! void f_impl(int i) { i_ += i; }
    // No g_impl() here! void g_impl(int i) { i_ += i; }
};

int main() {
    {
        B<D1>* b = new D1;
        b->f(42);
        b->g(42);
    }
    {
        B<D2>* b = new D2;
        b->f(42);
        // b->g(42); // Should not compile
    }
}
