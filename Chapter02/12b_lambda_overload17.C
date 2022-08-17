// C++17 lambda overloads
#include <iostream>

template <typename ... F>
struct overload_set : public F ... {
    overload_set(F&& ... f) : F(std::forward<F>(f)) ... {}
    using F::operator() ...;
};

template <typename ... F>
auto overload(F&& ... f) {
    return overload_set<F ...>(std::forward<F>(f) ...);
}

int main() {
    int i = 5;
    double d = 7.3;
    float f = 5.1;
    {
        auto l = overload(
                [](int* i) { std::cout << "i=" << *i << std::endl; },
                [](double* d) { std::cout << "d=" << *d << std::endl; }
                );
        l(&i);
        l(&d);
    }
    {
        auto l = overload(
                [](int i) { std::cout << "i=" << i << std::endl; },
                [](double d) { std::cout << "d=" << d << std::endl; }
                );
        l(i);
        l(d);
        l(f);
    }
}
