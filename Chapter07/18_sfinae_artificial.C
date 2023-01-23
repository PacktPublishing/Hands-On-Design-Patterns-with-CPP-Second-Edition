#include <iostream>
#include <vector>

template <typename U, typename V>
auto compare(U pu, V pv) -> decltype(bool(*pu == *pv)) {
  return *pu < *pv;
}

int main() {
    int i = 1;
    double d = 2.5;
    std::cout << compare(&i, &d) << std::endl;
    //compare(i, 1); // Does not compile - substitution failed
}
