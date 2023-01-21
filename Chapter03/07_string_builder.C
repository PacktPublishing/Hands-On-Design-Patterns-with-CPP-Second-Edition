#include <string>
#include <memory>
#include <iostream>

class Builder {
  std::string str_;
  public:
  Builder(std::initializer_list<char> l) : str_(l) {}
  std::string get(){ return std::move(str_); }
};

int main() {
    std::string s = Builder{{'a', 'b', 'c'}}.get();
    std::cout << s << std::endl;
}
