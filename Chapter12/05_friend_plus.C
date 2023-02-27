#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
    friend C operator+(const C& lhs, const C& rhs) { return C(lhs.x_ + rhs.x_); }
};

int main() {
    C x(1), y(2);
    std::cout << (x + y).get() << std::endl;
    std::cout << (x + 2).get() << std::endl;
    std::cout << (1 + y).get() << std::endl;
}
