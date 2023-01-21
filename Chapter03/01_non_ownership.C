#include <iostream>
#include <vector>

struct V : public std::vector<int> {
    using std::vector<int>::vector;
    ~V() { std::cout << "Deleting V" << std::endl; }
};

void Transmorgrify(V* v) {
    // v can be null, or we would have used a reference
    if (!v) return;
    // Operate on v, change it (non-const pointer!) but do not delete it
    for (auto& x : *v) x *= 2;
}

int main() {
    {
        V v {{ 1, 2, 3 }}; // I own v
        std::cout << "Transmogrify - v must not be deleted" << std::endl;
        Transmorgrify(&v); // You can work on it but I still own it
        std::cout << "Transmogrify done" << std::endl;
        std::cout << "v is deleted by the owner" << std::endl;
    } // I own v - I must delete it
}

