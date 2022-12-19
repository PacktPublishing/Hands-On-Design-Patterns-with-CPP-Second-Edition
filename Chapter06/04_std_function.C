#include <iostream>
#include <functional>
#include <string>

size_t f1(const std::string& s) { return s.capacity(); }

int main() {
    std::function<size_t(const std::string&)> f;

    f = f1;
    std::cout << f("abcde") << std::endl;
    
    char c = 'b';
    f = [=](const std::string& s) { return s.find(c); };
    std::cout << f("abcde") << std::endl;

    f = &std::string::size;
    std::cout << f("abcde") << std::endl;
}
