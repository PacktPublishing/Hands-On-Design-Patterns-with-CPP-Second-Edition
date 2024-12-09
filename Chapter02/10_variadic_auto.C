#include <iostream>

template <auto... Values> struct value_list {};

template <size_t N, auto... Values> struct nth_value_helper;
template <size_t n, auto v1, auto... Values>
struct nth_value_helper<n, v1, Values...> {
    static constexpr auto value = nth_value_helper<n - 1, Values...>::value;
};
template <auto v1, auto... Values>
struct nth_value_helper<0, v1, Values...> {
    static constexpr auto value = v1;
};

template <size_t N, auto... Values>
constexpr auto nth_value(value_list<Values...>) {
    return nth_value_helper<N, Values...>::value;
}

int main() {
#if not defined(__clang__)
    value_list<2, 3l, 4.2> vl;
    std::cout << nth_value<2>(vl) << std::endl;
#endif
}
