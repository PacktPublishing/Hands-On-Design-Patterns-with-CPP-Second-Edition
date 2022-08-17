// Template template parameter in a class template
#include <iostream>

template <typename T, template <typename> typename Container> class Builder {
    Container<T> data_;
    public:
    void add(const T& t) { data_.push_back(t); }
    void print() const { 
        for (auto x : data_) std::cout << x << " ";
        std::cout << std::endl;
    }
};

#include <vector>

template <typename T> class my_vector : public std::vector<T> {
    using vec_t = std::vector<T>;
    public:
    void push_back(const T& v) { vec_t::push_back(v); }
    auto begin() const { return vec_t::begin(); }
    auto end() const { return vec_t::end(); }
};

int main() {
    //Builder<int, std::vector> b; // May or may not compile
    Builder<int, my_vector> b;
    b.add(1);
    b.add(2);
    b.print();
}
