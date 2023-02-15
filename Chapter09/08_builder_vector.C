#include <vector>
#include <set>
#include <iostream>

class FibonacciBuilder {
    using V = std::vector<unsigned long>;
    V cache_ { 1, 1, 2, 3, 5 };

    public:
    // Compute first n Fibonacci numbers.
    V operator()(size_t n) {
        while (cache_.size() < n) {     // Cache new numbers
            cache_.push_back(cache_[cache_.size() - 1] + cache_[cache_.size() - 2]);
        }
        return V{cache_.begin(), cache_.begin() + n}; // Return cached numbers
    }
};

int main() {
    FibonacciBuilder b;

    auto fib10 = b(10);
    for (unsigned long i : fib10) std::cout << i << " ";
    std::cout << std::endl;

}
