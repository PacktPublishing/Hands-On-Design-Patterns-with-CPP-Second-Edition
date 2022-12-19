#include <stdlib.h>
#include <memory>
#include <iostream>

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
        std::cout << "Deleter invoked" << std::endl;
    }
};

int main() {
    MyHeap heap;
    std::shared_ptr<int> p(new(&heap) int(0), MyDeleter(&heap));
    std::shared_ptr<int> q(new int(1));
    q = p;
    std::cout << "End of scope, deletions to proceed..." << std::endl;
}

