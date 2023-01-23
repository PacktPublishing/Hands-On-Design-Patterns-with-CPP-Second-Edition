#include <type_traits>
#include <iostream>
#include <vector>

namespace detail {
template <typename T, typename = typename T::value_type> std::true_type test(int);
template <typename T> std::false_type test(...);     // failback
}

template <typename T> using has_value_type = decltype(detail::test<T>(0));
template <typename T> using has_value_type_t = has_value_type<T>::type;
template <typename T> inline constexpr bool has_value_type_v = has_value_type<T>::value;

struct A {
};
 
int main() {
  static_assert(has_value_type<std::vector<int>>::value, "");
  static_assert(!has_value_type_v<A>, "");
  std::cout << has_value_type<std::vector<int>>::value << std::endl;
  std::cout << has_value_type<int>::value << std::endl;
}

