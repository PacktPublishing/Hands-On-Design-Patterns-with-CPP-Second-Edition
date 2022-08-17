// Example of constexpr function
#include <iostream>

constexpr size_t length(const char* s) { size_t res = 0; while (*(s++)) ++res; return res; }

int main() {
    std::cout << length("abc") << std::endl;
    static_assert(length("abc") == 3, "");
    char s[] = "runtime";
    std::cout << length(s) << std::endl;
    //static_assert(length(s) == 7, ""); // Not consteval
}
