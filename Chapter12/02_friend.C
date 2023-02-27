#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
    friend C increase(C c, int dx);
};

C increase(C c, int dx) {
    return C(c.x_ + dx);	// Now it compiles
}

int main() {
    C c1(1);
    C c2 = increase(c1, 1);
    std::cout << c2.get() << std::endl;
}
