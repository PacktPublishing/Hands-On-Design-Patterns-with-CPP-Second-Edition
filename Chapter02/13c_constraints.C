// Constraint with a concept requirement
#include <iostream>
#include <type_traits>

template <typename T1, typename T2> concept Comparable = requires(T1 t1, T2 t2) { t1 < t2; };

template <typename T1, typename T2> std::common_type_t<T1, T2> min2(T1&& t1, T2&& t2)
    requires Comparable<T1, T2>
{
    if (t1 < t2) return std::forward<T1>(t1);
    return std::forward<T2>(t2);
}

struct S1 {
    int i;
    S1(int i) : i(i) {}
    friend std::ostream& operator<<(std::ostream& out, S1 s) { out << s.i; return out; }
};

struct S2 {
    int i;
    S2(int i) : i(i) {}
    friend bool operator<(S2 a, S2 b) { return a.i < b.i; }
    friend std::ostream& operator<<(std::ostream& out, S2 s) { out << s.i; return out; }
};

int main() {
    std::cout << min2(1, 2) << std::endl;
    static_assert(std::is_same_v<decltype(min2(1, 2)), int>);

    std::cout << min2(1, 2l) << std::endl;
    static_assert(std::is_same_v<decltype(min2(1, 2l)), long>);

    std::cout << min2(1.1, 2) << std::endl;
    static_assert(std::is_same_v<decltype(min2(1.1, 2)), double>);

    //std::cout << min2(S1(1), S1(2)) << std::endl; // No operator <
    std::cout << min2(S2(1), S2(2)) << std::endl;
    static_assert(std::is_same_v<decltype(min2(S2(1), S2(2))), S2>);

    std::cout << min2(1, S2(2)) << std::endl;
    static_assert(std::is_same_v<decltype(min2(1, S2(2))), S2>);
}
