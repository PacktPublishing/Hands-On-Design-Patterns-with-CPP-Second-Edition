#include <string>
#include <string_view>
#include <memory>
#include <iostream>

void UseString(std::string_view s) {
  for (char c : s) std::cout << c;
  std::cout << std::endl;
};

void ConsumeString(std::string&& s) {
    std::string ss(std::move(s));
    std::cout << "String " << ss << " consumed" << std::endl;
}

int main() {
    std::string s = "abc";
    UseString(s);
    UseString("xyz");
    ConsumeString(std::move(s));
}
