#include <iostream>
#include <vector>

void f1(auto x) { std::cout << x << std::endl; }
auto f2(auto x) -> decltype(x) { return x; }
void f3(const auto& x) { std::cout << x << std::endl; }
//size_t f4(const std::vector<auto>& v) { return v.size(); }

int main() {
    f1(1);
    std::cout << f2(2) << std::endl;
    f3(3);
    //std::cout << f4(std::vector<int>{1, 2, 3, 4}) << std::endl;
}
