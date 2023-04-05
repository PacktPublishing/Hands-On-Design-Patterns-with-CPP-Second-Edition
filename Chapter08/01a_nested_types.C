#include <iostream>

template <typename D, typename value_type> class B {
    public:
    //typename D::result_type f() { return i; }         // Incomplete type - does not compile
    auto f() {
        using res_t = typename D::result_type;          // OK inside function body
        return res_t{i};                                // Deducing auto == res_t
    }
    //typename B::value_type i;                         // Incomplete type - does not compile
    value_type i {};
};

class D : public B<D, int> {
    public:
    using result_type = int;
    using value_type = int;
};

int main() {
    D d;
    d.i = 1;
    std::cout << d.f() << std::endl;
}
