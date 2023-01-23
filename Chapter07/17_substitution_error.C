#include <iostream>
#include <vector>

template <typename T, typename V>
bool find(const T& cont, V val) {
    for (typename T::value_type x : cont) if (x == val) return true;
    return false;
}

template <typename U, typename V>
auto compute(U u, V v) {
    std::common_type_t<U, V> res = (u > v) ? u : v;
    return res;
}

template <typename T>
auto process(T p) -> decltype(*p) {
    return *p;
}

int main() {
    std::vector<int> v { 1, 1, 2, 3, 5, 8, 13, 21 };
    std::cout << find(v, 2) << " " << find(v, 7) << std::endl;
    int a[] = { 1, 2, 3 }; (void)a;
    //find(a, 2); // Does not compile - error

    std::cout << compute(1, 2.5) << std::endl;
    //compute(v, 1); // Does not compile

    std::cout << process(v.begin()) << std::endl;
    //process(0); // Does not compile
}
