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

// Macros for declaring concepts - XVALUE(T, name) declares an argument of type T, "name" is the name of the argument.
//   CLVALUE - const lvalue reference
//   LVALUE  - con-const lvalue reference
//   RVALUE  - rvalue reference
#define CLVALUE(TYPE, NAME) const TYPE& NAME = Concepts::concept_detail::clvalue<TYPE>()
#define LVALUE(TYPE, NAME) TYPE& NAME = Concepts::concept_detail::lvalue<TYPE>()
#define RVALUE(TYPE, NAME) TYPE&& NAME = Concepts::concept_detail::rvalue<TYPE>()

// Utility for defining concepts of one, two, etc type arguments.
// In C++17, "static" can be replaced by "inline".
struct ConceptArg {};
#define DECLARE_CONCEPT(NAME, SUFFIX) \
     template <typename... T> constexpr inline auto NAME ## SUFFIX(Concepts::ConceptArg, int) -> decltype(NAME ## SUFFIX<T...>(), bool{}) { return true; } \
     template <typename... T> constexpr inline bool NAME ## SUFFIX(Concepts::ConceptArg, ...) { return false; } \
     template <typename... T> constexpr static bool NAME = NAME ## SUFFIX<T...>(Concepts::ConceptArg{}, 0)
// Why ConceptArg? So NAME<T...> cannot be ambiguous with NAME(...) or NAME(int).
} // namespace Concepts

// REQUIRES clause for requiring concepts in template functions:
// template <typename T1, typename T2, ..., REQUIRES(HasConcept1<T>() && !HasConcept2<T2>())> void f(...)
#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__), bool> = true

// 
#define CONCEPT constexpr inline auto

namespace Concepts {

template <typename U, typename V> CONCEPT AddableTest(CLVALUE(U, u), CLVALUE(V, v)) -> decltype(u + v);
DECLARE_CONCEPT(Addable, Test);

template <typename R> CONCEPT RangeTest(RVALUE(R, r)) -> decltype(std::begin(r), std::end(r));
DECLARE_CONCEPT(Range, Test);

template <typename T> CONCEPT IndexWritableTest(LVALUE(T, t)) -> decltype(t[0] = t[0]);
DECLARE_CONCEPT(IndexWritable, Test);

} // namespace Concepts

template <typename R, REQUIRES(Concepts::Range<R>)> void sort1(R&&) { std::cout << "Sorting range 1" << std::endl; }

template <typename R> CONCEPT Sort1CompilesTest(RVALUE(R, r)) -> decltype(sort1(r));
DECLARE_CONCEPT(Sort1Compiles, Test);

int main() {
    std::vector<int> v{ 3, 2, 1 };
    static_assert(Concepts::Range<decltype(v)>, "");
    static_assert(!Concepts::Range<decltype(0)>, "");
    sort1(v);
    //sort1(0); // Not a range - does not compile
    static_assert(Sort1Compiles<decltype(v)>, "");
    static_assert(!Sort1Compiles<int>, "");

    static_assert(Concepts::Addable<int, double>, "");
    static_assert(!Concepts::Addable<int*, double>, "");

    static_assert(Concepts::IndexWritable<decltype(v)>, "");
    static_assert(!Concepts::IndexWritable<const std::vector<int>>, "");
    static_assert(!Concepts::IndexWritable<int>, "");
    static_assert(Concepts::IndexWritable<int*>, "");
    static_assert(!Concepts::IndexWritable<const int*>, "");
}
