#include <iostream>
#include <vector>
#include <type_traits>

template <typename T> struct S {
    typename T::value_type f() { return 0; }
};

int main() {
    S<std::vector<int>> sv;
    sv.f();
    //S<int> si; // Does not compile
}
