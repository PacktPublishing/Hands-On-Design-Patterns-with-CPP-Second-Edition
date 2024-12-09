#include <type_traits>
#include <iostream>
#include <vector>

namespace detail {
template <typename T> std::true_type test(int T::*); // selected if T is a class type
template <typename T> std::false_type test(...);     // selected otherwise
}

template <typename T> using is_class = decltype(detail::test<T>(nullptr));
template <typename T> using is_class_t = typename is_class<T>::type;
template <typename T> inline constexpr bool is_class_v = is_class<T>::value;

struct A {
};

union U {
  int i;
  char c;
};

int main() {
  static_assert(is_class<std::vector<int>>::value, "");
  static_assert(is_class_v<A>, "");
  static_assert(!is_class_v<int>, "");
  std::cout << is_class<std::vector<int>>::value << std::endl;
  std::cout << is_class<A>::value << std::endl;
  std::cout << is_class<int>::value << std::endl;

  std::cout << is_class_v<U> << " " << std::is_class_v<U> << std::endl;
}
