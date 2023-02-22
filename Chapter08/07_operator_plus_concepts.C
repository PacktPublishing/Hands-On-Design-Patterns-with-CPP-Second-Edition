#include <iostream>

template <typename T> inline constexpr bool gen_plus = false;

#define V3
#if defined(V1)
template <typename T> 
T operator+(const T& lhs, const T& rhs) { T tmp = lhs; tmp += rhs; return tmp; }
#elif defined(V2)
template <typename T> 
requires( requires(T a, T b) { a += b; } )
T operator+(const T& lhs, const T& rhs) { T tmp = lhs; tmp += rhs; return tmp; }
#elif defined(V3)
template <typename T> 
requires gen_plus<T>
T operator+(const T& lhs, const T& rhs) { T tmp = lhs; tmp += rhs; return tmp; }
#endif

class D {
    int i_;
    public:
    explicit D(int i) : i_(i) {}
    D& operator+=(const D& rhs) { i_ += rhs.i_; return *this; }
    friend std::ostream& operator<<(std::ostream& out, const D& d) { out << d.i_; return out; }
};
template <> inline constexpr bool gen_plus<D> = true;

class C {
    int i_;
    public:
    explicit C(int i) : i_(i) {}
    friend std::ostream& operator<<(std::ostream& out, const C& c) { out << c.i_; return out; }
};

int main() {
    D d1(1);
    D d2(7);
    D d3(42);
    std::cout << "d1: " << d1 << ", d2: " << d2 << ", d3: " << d3 << std::endl;
    std::cout << "d1 + d2: " << (d1 + d2) << " " << std::endl;
    std::cout << "d1 + d3: " << (d1 + d3) << " " << std::endl;

    C c1(1), c2(2);
    //std::cout << "c1 + c2: " << (c1 + c2) << " " << std::endl; // Does not compile
}
