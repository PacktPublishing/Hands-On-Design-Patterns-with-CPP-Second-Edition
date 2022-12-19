#include <any>
#include <iostream>

int main() {
    std::any a(5);
    std::cout << std::any_cast<long>(a) << std::endl;
}
