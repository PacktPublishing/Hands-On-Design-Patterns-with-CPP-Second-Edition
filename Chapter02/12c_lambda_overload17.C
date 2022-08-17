// C++17 lambda overloads using deduction guides
#include <iostream>

template <typename ... F>
struct overload : public F ... {
    using F::operator() ...;
};

template <typename ... F> // Deduction guide
overload(F&& ... ) -> overload<F ...>;

int main() {
    int i = 5;
    double d = 7.3;
    float f = 5.1;
    {
        auto l = overload{
                [](int* i) { std::cout << "i=" << *i << std::endl; },
                [](double* d) { std::cout << "d=" << *d << std::endl; },
                };
        l(&i);
        l(&d);
    }
    {
        auto l = overload{
                [](int i) { std::cout << "i=" << i << std::endl; },
                [](double d) { std::cout << "d=" << d << std::endl; },
                };
        l(i);
        l(d);
        l(f);
    }
}
