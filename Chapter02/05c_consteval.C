// Forcing consteval evaluation (C++17)
#include <iostream>

template <auto V>
static constexpr auto force_consteval = V;

constexpr size_t length(const char* s) { size_t res = 0; while (*(s++)) ++res; return res; }

int main() {
    std::cout << force_consteval<length("abc")> << std::endl;
    char s[] = "runtime"; (void)s;
    //std::cout << force_consteval<length(s)> << std::endl;
}
