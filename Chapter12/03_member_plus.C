#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
    C operator+(const C& rhs) const { return C(x_ + rhs.x_); }
};

int main() {
    C x(1), y(2);
    std::cout << (x + y).get() << std::endl;
    std::cout << (x + 2).get() << std::endl;
    //std::cout << (1 + y).get() << std::endl; // Does not compile
}
