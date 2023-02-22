#include <iostream>

template <typename D> struct compare_base {
    friend bool operator!=(const D& lhs, const D& rhs) { return !(lhs == rhs); }
    friend bool operator<=(const D& lhs, const D& rhs) { return lhs.cmp(rhs) <= 0; }
    friend bool operator>=(const D& lhs, const D& rhs) { return lhs.cmp(rhs) >= 0; }
    friend bool operator< (const D& lhs, const D& rhs) { return lhs.cmp(rhs) <  0; }
    friend bool operator> (const D& lhs, const D& rhs) { return lhs.cmp(rhs) >  0; }
};

class D : public compare_base<D> {
    int i_;
    public:
    explicit D(int i) : i_(i) {}
    auto cmp(const D& rhs) const { return (i_ < rhs.i_) ? -1 : ((i_ > rhs.i_) ? 1 : 0); }
    bool operator==(const D& rhs) const { return i_ == rhs.i_; }
    friend std::ostream& operator<<(std::ostream& out, const D& d) { out << d.i_; return out; }
};

int main() {
    D d1(1);
    D d2(1);
    D d3(42);
    std::cout << "d1: " << d1 << ", d2: " << d2 << ", d3: " << d3 << std::endl;
    std::cout << "d1 == d2: " << (d1 == d2) << " " << std::endl;
    std::cout << "d1 <= d2: " << (d1 <= d2) << " " << std::endl;
    std::cout << "d1 <  d2: " << (d1 <  d2) << " " << std::endl;
    std::cout << "d1 == d3: " << (d1 == d3) << " " << std::endl;
    std::cout << "d1 <= d3: " << (d1 <= d3) << " " << std::endl;
    std::cout << "d1 <  d3: " << (d1 <  d3) << " " << std::endl;
}
