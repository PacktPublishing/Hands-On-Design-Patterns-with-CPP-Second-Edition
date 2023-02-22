#include <iostream>

template <typename D> class B {
    public:
    B() : i_(0) {}
    virtual ~B() {}
    void f(int i) {
        std::cout << "B::f() calling D::f()..." << std::endl;
        static_cast<D*>(this)->f(i);
        std::cout << "B::f() done!" << std::endl;
    }
    int get() const { return i_; }
    protected:
    int i_;
};
class D_bad : public B<D_bad> {
    public:
    // No f() here! void f(int i) { i_ += i; }
};
class D_ok : public B<D_ok> {
    public:
    void f(int i) {
        std::cout << "Running D::f()..." << std::endl;
        i_ += i;
        std::cout << "D::f() done" << std::endl;
    }
};

int main() {
    {
        B<D_ok>* b = new D_ok;
        b->f(42);
    }
    {
        B<D_bad>* b = new D_bad;
        b->f(42);
    }
}
