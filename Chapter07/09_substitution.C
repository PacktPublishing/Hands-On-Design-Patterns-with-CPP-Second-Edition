#include <iostream>
#include <memory>

template <typename T> T* f(T i, T& j) {
  j = 2*i;
  return new T(i);
}

int main() {
    int i = 5, j = 7;
    std::unique_ptr<const int> p(f(i, j));
    std::cout << *p << std::endl;
}
