#include <string>
#include <memory>
#include <iostream>

std::string MakeString(std::initializer_list<char> l) {
  std::string s(l);
  return s;
};

int main() {
    std::string s = MakeString({'a', 'b', 'c'});
    std::cout << s << std::endl;
}
