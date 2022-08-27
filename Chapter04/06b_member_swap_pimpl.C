#include <utility>
#include <iostream>

class C_impl;
class C {
    public:
    explicit C(int i, int j);
    ~C();
    void swap(C& rhs) noexcept {
        std::cout << "C::swap" << std::endl;
        using std::swap;
        swap(pimpl_, rhs.pimpl_);
    }
    C_impl* pimpl_;
};

class C_impl {
    public:
    explicit C_impl(int i, int j) : i_(i), j_(j) {}
    int i_;
    int j_;
};

C::C(int i, int j) : pimpl_(new C_impl(i, j)) {}
C::~C() { delete pimpl_; }

int main() {
    C c1(1, 10), c2(2, 20);
    std::cout << "\nInitial:" << std::endl;
    std::cout << "c1: " << c1.pimpl_->i_ << " c2: " << c2.pimpl_->i_ << std::endl;

    c1.swap(c2);
    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "c1: " << c1.pimpl_->i_ << " c2: " << c2.pimpl_->i_ << std::endl;
}
