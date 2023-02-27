#include <iostream>

class C {
    static int n_;
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
    friend int f(int i) { return i + C::n_; }
    friend int g(const C& c) { return c.x_ + C::n_; }
};
int C::n_ = 42;

int main() {
    C c(1);
    //std::cout << f(0) << std::endl; // Does not comppile - no ADL
    //std::cout << C::f(0) << std::endl; // Does not comppile - not a member
    std::cout << g(c) << std::endl;
}
