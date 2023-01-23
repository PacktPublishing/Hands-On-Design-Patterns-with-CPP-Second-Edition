#include <vector>
#include <iostream>

// REQUIRES clause for requiring concepts in template functions:
// template <typename T1, typename T2, ..., REQUIRES(HasConcept1<T>() && !HasConcept2<T2>())> void f(...)
#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__), bool> = true

namespace Concepts {

template <typename R> constexpr static auto RangeTest(R* r) -> decltype(std::begin(*r), std::end(*r) , bool{}) { return true; }
template <typename R> constexpr bool RangeTest(...) { return false; }
template <typename R> constexpr static bool Range = RangeTest<R>(nullptr);

} // namespace Concepts

template <typename R, REQUIRES(Concepts::Range<std::decay_t<R>>)> void sort1(R&&) { std::cout << "Sorting range 1" << std::endl; }

int main() {
    std::vector<int> v{ 3, 2, 1 };
    static_assert(Concepts::Range<decltype(v)>, "");
    static_assert(!Concepts::Range<decltype(0)>, "");
    sort1(v);
    //sort1(0); // Not a range - does not compile

}
