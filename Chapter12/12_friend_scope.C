#include <iostream>

class C {
    static int n_;
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
    friend int f(int i);
    friend int g(const C& c) { return c.x_ + C::n_; }
};
int C::n_ = 42;

int f(int i) { return i + C::n_; }

int main() {
    C c(1);
    std::cout << f(0) << std::endl;
    std::cout << g(c) << std::endl;
}
