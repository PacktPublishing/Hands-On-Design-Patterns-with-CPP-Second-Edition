#include <type_traits>
#include <iostream>
#include <vector>

namespace detail {
template <typename T, std::enable_if_t<sizeof(typename T::value_type) != 0, bool> = true> std::true_type test(int);
template <typename T> std::false_type test(...);     // failback
}

template <typename T> using has_value_type = decltype(detail::test<T>(0));

struct A {
};
 
int main() {
  static_assert(has_value_type<std::vector<int>>::value, "");
  static_assert(!has_value_type<A>::value, "");
  std::cout << has_value_type<std::vector<int>>::value << std::endl;
  std::cout << has_value_type<int>::value << std::endl;
}

