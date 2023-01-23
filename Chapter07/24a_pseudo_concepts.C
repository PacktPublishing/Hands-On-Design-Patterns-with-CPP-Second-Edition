#include <vector>
#include <iostream>

namespace Concepts {

// The functions lvalue() and rvalue() are similar to std::declval() but can be used as default arguments for functions.
// The functions that use these arguments cannot be invoked except inside decltype() and other compile-time contexts.
// The body of the function should not be required if it's used only in decltype(), but it is required.
// This is a bug in the standard, not in the compiler, and is fixed in C++20
// (see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0859r0.html)
namespace concept_detail {
template <typename T> constexpr const T& clvalue() { abort(); }
template <typename T> constexpr T& lvalue() { abort(); }
template <typename T> constexpr T&& rvalue() { abort(); }
} // namespace concept_detail

// REQUIRES clause for requiring concepts in template functions:
// template <typename T1, typename T2, ..., REQUIRES(HasConcept1<T>() && !HasConcept2<T2>())> void f(...)
#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__), bool> = true
} // namespace Concepts

namespace Concepts {

template <typename R> constexpr static auto RangeTest(R& r = Concepts::concept_detail::lvalue<R>()) -> decltype(std::begin(r), std::end(r));
template <typename R> constexpr static auto RangeTest(int) -> decltype(RangeTest<R>(), bool{}) { return true; }
template <typename R> constexpr bool RangeTest(...) { return false; }
template <typename R> constexpr static bool Range = RangeTest<R>(0);

} // namespace Concepts

template <typename R, REQUIRES(Concepts::Range<R>)> void sort1(R&&) { std::cout << "Sorting range 1" << std::endl; }

int main() {
    std::vector<int> v{ 3, 2, 1 };
    static_assert(Concepts::Range<decltype(v)>, "");
    static_assert(!Concepts::Range<decltype(0)>, "");
    sort1(v);
    //sort1(0); // Not a range - does not compile

}
