#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
    friend C operator+(const C& lhs, const C& rhs);
};

C operator+(const C& lhs, const C& rhs) { return C(lhs.x_ + rhs.x_); }

std::ostream& operator<<(std::ostream& out, const C& c) {
    out << c.get();
    return out;
}

C f(const C& x, const C& y) {
    return x + y;
}

int main() {
    C x(5), y(7);
    std::cout << (x + y) << std::endl;
    std::cout << (x + 3) << std::endl;
    std::cout << (3 + y) << std::endl;
}
