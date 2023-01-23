#include <iostream>
#include <vector>
#include <type_traits>

template <typename T> struct S {
    template <typename U = T>
    std::enable_if_t<std::is_same_v<T, U>, typename U::value_type>
    f() { return 0; }
};

int main() {
    S<std::vector<int>> sv;
    sv.f();
    S<int> si; (void)si;
    //si.f(); // Does not compile - SFINAE, no function f() to call
}
