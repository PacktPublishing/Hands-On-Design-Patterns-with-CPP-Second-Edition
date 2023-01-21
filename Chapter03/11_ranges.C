#include <string>
#include <vector>
#include <list>
#include <ranges>
#include <memory>
#include <iostream>

void UseData(std::ranges::view auto data) {
  for (const auto& x : data) std::cout << x;
  std::cout << std::endl;
};

void ModifyData(std::ranges::view auto data) {
  for (auto& x : data) ++x;
};

int main() {
    std::string s = "abc";
    UseData(std::views::all(s));
    UseData(std::views::all("xyz"));
    ModifyData(std::views::all(s));
    UseData(std::views::all(s));

    std::vector<int> v = { 1, 2, 3 };
    UseData(std::views::all(v));
    ModifyData(std::views::all(v));
    UseData(std::views::all(v));

    std::list<int> l = { 5, 6, 7, 8 };
    UseData(std::views::all(l));
    ModifyData(std::views::all(l));
    UseData(std::views::all(l));
}
