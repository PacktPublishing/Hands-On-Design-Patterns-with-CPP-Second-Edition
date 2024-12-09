#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
};

C increase(C c, int dx) {
    // return C(c.x_ + dx);	// Does not compile
    return C(c.get() + dx);
}

int main() {
    C c1(1);
    C c2 = increase(c1, 1);
    std::cout << c2.get() << std::endl;
}
