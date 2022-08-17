#include <iostream>

template <typename T, size_t N> class Array {
    public:
    T& operator[](size_t i) {
        if (i >= N) throw std::out_of_range("Bad index");
        return data_[i];
    }
    constexpr size_t size() const { return N; }
    private:
    T data_[N];
};

int main() {
    Array<int, 4> a;
    a[0] = 0;
    for (size_t i = 1; i != a.size(); ++i) a[i] = a[i - 1] + 1;
    for (size_t i = 0; i != a.size(); ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
}
