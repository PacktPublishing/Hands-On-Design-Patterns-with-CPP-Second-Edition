#include <iostream>

template <typename D> struct plus_base {
    friend D operator+(const D& lhs, const D& rhs) { D tmp = lhs; tmp += rhs; return tmp; }
};

class D : public plus_base<D> {
    int i_;
    public:
    explicit D(int i) : i_(i) {}
    D& operator+=(const D& rhs) { i_ += rhs.i_; return *this; }
    friend std::ostream& operator<<(std::ostream& out, const D& d) { out << d.i_; return out; }
};

int main() {
    D d1(1);
    D d2(7);
    D d3(42);
    std::cout << "d1: " << d1 << ", d2: " << d2 << ", d3: " << d3 << std::endl;
    std::cout << "d1 + d2: " << (d1 + d2) << " " << std::endl;
    std::cout << "d1 + d3: " << (d1 + d3) << " " << std::endl;
}
