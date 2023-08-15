// Template template parameter in a function template
#include <iostream>

template <template <typename> class Container, typename T>
void print(const Container<T>& container) {
    for (auto x : container) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

template <template <typename> class Out_container,
         template <typename> class In_container,
         typename T>
Out_container<T> resequence(const In_container<T>& in_container) {
    Out_container<T> out_container;
    for (auto x : in_container) {
        out_container.push_back(x);
    }
    return out_container;
}

#include <vector>
#include <deque>

int main() {
    std::vector<int> v{ 1, 2, 3, 4, 5 };
#if not defined(__clang__) && not defined(_MSC_VER)
    print(v);
    auto d = resequence<std::deque>(v);
    print(d);
#endif
}
