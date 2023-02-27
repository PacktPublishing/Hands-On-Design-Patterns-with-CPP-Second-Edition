#include <iostream>
#include <cstdlib>
#include <new>

class Alloc {
    void* alloc(size_t s) { return malloc(s); }
    void dealloc(void* p) { free(p); }
    friend void* operator new (size_t s, Alloc* a);
    friend void operator delete (void* p, Alloc* a) { a->dealloc(p); }
};
void* operator new (size_t s, Alloc* a) { return a->alloc(s); }

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    friend std::ostream& operator<<(std::ostream& out, const C& c) {
        out << c.x_;
        return out;
    }
};

int main() {
    Alloc a;
    C* c = new(&a) C(42);
    std::cout << "Allocator @ " << &a << ", object @ " << c << std::endl;
    if (static_cast<void*>(&a) == static_cast<void*>(c)) std::cout << "Rules for operator new lookup are not followed" << std::endl;
}
