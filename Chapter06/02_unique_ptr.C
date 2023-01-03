#include <stdlib.h>
#include <memory>

class MyHeap {
    public:
    void* allocate(size_t size) { return malloc(size); }
    void deallocate(void* p) { free(p); }
};
void* operator new(size_t size, MyHeap* heap) {
    return heap->allocate(size);
}

class MyDeleter {
    MyHeap* heap_;
    public:
    MyDeleter(MyHeap* heap) : heap_(heap) {}
    template <typename T> void operator()(T* p) {
        p->~T();
        heap_->deallocate(p);
    }
};

int main() {
    MyHeap heap;
    std::unique_ptr<int, MyDeleter> p(new(&heap) int(0), MyDeleter(&heap));
    // std::unique_ptr<int> q = p; // Does not compile!
}

