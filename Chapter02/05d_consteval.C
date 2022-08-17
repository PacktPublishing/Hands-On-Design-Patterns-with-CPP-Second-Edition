// Forcing consteval evaluation (C++14)
#include <iostream>

template <typename T, T V>
static constexpr auto force_consteval_helper = V;
#define force_consteval(V) force_consteval_helper<decltype(V), (V)>

constexpr size_t length(const char* s) { size_t res = 0; while (*(s++)) ++res; return res; }

int main() {
    std::cout << force_consteval(length("abc")) << std::endl;
    char s[] = "runtime"; (void)s;
    //std::cout << force_consteval(length(s)) << std::endl;
}
