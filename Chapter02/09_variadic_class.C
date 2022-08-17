#include <iostream>

template <typename ... T> struct Group;
template <typename T1> struct Group<T1> {
    T1 t1_;
    Group() = default;
    explicit Group(const T1& t1) : t1_(t1) {}
    explicit Group(T1&& t1) : t1_(std::move(t1)) {}
    explicit operator const T1&() const { return t1_; }
    explicit operator T1&() { return t1_; }
};
template <typename T1, typename ... T>
struct Group<T1, T ...> : Group<T ...> {
    T1 t1_;
    Group() = default;
    explicit Group(const T1& t1, T&& ... t) : Group<T ...>(std::forward<T>(t) ...), t1_(t1) {}
    explicit Group(T1&& t1, T&& ... t) : Group<T...>(std::forward<T>(t)...), t1_(std::move(t1)) {}
    explicit operator const T1&() const { return t1_; }
    explicit operator T1&() { return t1_; }
};

template <typename ... T> Group(T&&... t) -> Group<T...>;
template <typename ... T> auto makeGroup(T&& ... t) { return Group<T ...>(std::forward<T>(t) ...); }

int main() {
    Group<int, long> g1(1, 2);
    std::cout << int(g1) << std::endl;
    Group g2(3, 2.2, std::string("xyz"));
    std::cout << int(g2) << std::endl;
    std::cout << double(g2) << std::endl;
    std::cout << std::string(g2) << std::endl;
    auto g3 = makeGroup(3, 2.2, std::string("xyz"));
    std::cout << int(g3) << std::endl;
    std::cout << double(g3) << std::endl;
    std::cout << std::string(g3) << std::endl;
}
