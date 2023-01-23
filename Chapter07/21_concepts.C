#include <vector>
#include <iostream>
#include <ranges>
#include <algorithm>

template <typename R> concept Range = requires(R r) { std::begin(r); std::end(r); };

template <typename R> requires(Range<R>) void sort1(R&& r) { std::cout << "Sorting range 1" << std::endl; std::sort(std::begin(r), std::end(r)); }
template <Range R> void sort2(R&& r) { std::cout << "Sorting range 2" << std::endl; std::sort(std::begin(r), std::end(r)); }
void sort3(Range auto&& r) { std::cout << "Sorting range 3" << std::endl; std::sort(std::begin(r), std::end(r)); }

template <Range R> struct sorter {
    R& range;
    sorter(R& r) : range(r) {}
    void sort() { std::sort(std::begin(range), std::end(range)); }
};
template <Range R> sorter(R&& r) -> sorter<R>;

template <typename T> struct holder {
    T& value;
    holder(T& t) : value(t) {}
    void sort() requires(Range<T>) { std::sort(std::begin(value), std::end(value)); }
};
template <typename T> holder(T&& t) -> holder<T>;

int main() {
    std::vector<int> v{ 3, 2, 1 };
    static_assert(Range<std::vector<int>>);
    static_assert(!Range<int>);
    sort1(v);
    //sort1(0); // Not a range
    sort2(v);
    sort3(v);

    int a[] { 3, 2, 1 };
    sort1(a);
    sort2(a);
    sort3(a);

    sorter rv(v);
    rv.sort();

    sorter ra(a);
    ra.sort();

    int i = 5; (void)i;
    //sorter<int> fi(i); // Constraint not satisfied

    holder hv(v);
    hv.sort();

    holder ha(a);
    //ha.sort(); // C arrays are hard to use in templates ...
    // If you really need this to work, a partial specialization is much simpler than the alternative

    holder<int> hi(i);
    //hi.sort(); // Constraint not satisfied
}
