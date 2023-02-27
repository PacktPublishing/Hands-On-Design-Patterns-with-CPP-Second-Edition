#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    friend C operator+(const C& lhs, const C& rhs);
    friend std::ostream& operator<<(std::ostream& out, const C& c);
};
C operator+(const C& lhs, const C& rhs) { return C(lhs.x_ + rhs.x_); }
std::ostream& operator<<(std::ostream& out, const C& c) {
    out << c.x_;
    return out;
}

int main() {
    C x(5), y(7);
    std::cout << (x + y) << std::endl;
    std::cout << (x + 3) << std::endl;
    std::cout << (3 + y) << std::endl;
}
