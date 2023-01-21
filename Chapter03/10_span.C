#include <string>
#include <vector>
#include <span>
#include <memory>
#include <iostream>

void UseString(std::span<const char> s) {
  for (char c : s) std::cout << c;
  std::cout << std::endl;
};

void ModifyString(std::span<char> s) {
  for (char& c : s) ++c;
};

void UseData(std::span<const int> s) {
  for (int i : s) std::cout << i;
  std::cout << std::endl;
};

void ModifyData(std::span<int> s) {
  for (int& i : s) ++i;
};

int main() {
    std::string s = "abc";
    UseString(s);
    UseString("xyz");
    ModifyString(s);
    UseString(s);

    std::vector<int> v = { 1, 2, 3 };
    UseData(v);
    ModifyData(v);
    UseData(v);
}
