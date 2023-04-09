// Version 13 with move policy.
#include <cstdlib>
#include <utility>
#include <cassert>
#include <iostream>

// From Chapter 8, example 25
#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__), bool> = true

template <typename T>
struct DeleteByOperator {
    void operator()(T* p) const { 
        delete p;
    }
};

template <typename T>
struct DeleteByFree {
    void operator()(T* p) const {
        p->~T();
        free(p);
    }
};

template <typename T>
struct DeleteDestructorOnly {
    void operator()(T* p) const {
        p->~T();
    }
};

class SmallHeap {
    public:
    SmallHeap() {}
    ~SmallHeap() {}
    void* allocate(size_t s) {
        assert(s <= size_);
        return mem_;
    }
    void deallocate(void* p) {
        assert(p == mem_);
    }
    private:
    static constexpr size_t size_ = 1024;
    char mem_[size_];
    SmallHeap(const SmallHeap&) = delete;
    SmallHeap& operator=(const SmallHeap&) = delete;
};
void* operator new(size_t s, SmallHeap* h) { return h->allocate(s); }

template <typename T>
struct DeleteSmallHeap {
    explicit DeleteSmallHeap(SmallHeap& heap)
        : heap_(&heap) {}
    void operator()(T* p) const {
        p->~T();
        heap_->deallocate(p);
    }
    DeleteSmallHeap(DeleteSmallHeap&& that)
        : heap_(that.heap_) 
    {
        that.heap_ = nullptr;
    }
    DeleteSmallHeap(const DeleteSmallHeap&) = delete;
    DeleteSmallHeap& operator=(const DeleteSmallHeap&) = delete;
    private:
    SmallHeap* heap_;
};

struct MoveForbidden {
    static constexpr bool enabled = false;
};

struct MoveAllowed {
    static constexpr bool enabled = true;
};

template <typename T, typename DeletionPolicy = DeleteByOperator<T>,
                      typename MovePolicy = MoveForbidden>
class SmartPtr : private DeletionPolicy { // Empty base class optimization
    T* p_;
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy())
        : DeletionPolicy(std::move(deletion_policy)), p_(p)
    {}
    ~SmartPtr() { 
        DeletionPolicy::operator()(p_);
    }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
    template <typename U = T, REQUIRES(sizeof(U) != 0 && MovePolicy::enabled)>  // Pseudo-concepts
    SmartPtr(SmartPtr&& that) :
        DeletionPolicy(std::move(that)),
        p_(std::exchange(that.p_, nullptr))
    {}
    template <typename U = T, std::enable_if_t<sizeof(U) != 0 && MovePolicy::enabled, bool> = true>     // Explicit std::enable_if
    SmartPtr& operator=(SmartPtr&& that) {
        if (this == &that) return *this;
        DeletionPolicy::operator()(p_);
        p_ = std::exchange(that.p_, nullptr);
        DeletionPolicy::operator=(std::move(that));
        return *this;
    }
};

class C {
    int i_ {};
    public:
    explicit C(int i) : i_(i) { std::cout << "Constructing C(" << i << ") @" << this << std::endl; }
    ~C() { std::cout << "Destroying C @" << this << std::endl; }
    int get() const { return i_; }
};

int main() {
    {
        SmartPtr<C> c(new C(42));
        //SmartPtr<C> c1(std::move(c));  // Does not compile
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
        SmartPtr<C> c2;
        //c2 = std::move(c); // Does not compile
    }

    {
        SmallHeap h;
        SmartPtr<C, DeleteSmallHeap<C>> c{new(&h) C(42), DeleteSmallHeap<C>(h)};
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
    }

    {
        // C++17 does not help here since MovePolicy cannot be deduced from the constructor
        SmartPtr<C, DeleteByOperator<C>, MoveAllowed> c{new C(42)};
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
        auto c1(std::move(c));
        //std::cout << "C: " << c->get() << " @ " << &*c << std::endl; // This will crash
        std::cout << "C: " << c1->get() << " @ " << &*c1 << std::endl;
        SmartPtr<C, DeleteByOperator<C>, MoveAllowed> c2;
        c2 = std::move(c1);
        //std::cout << "C: " << c1->get() << " @ " << &*c1 << std::endl; // This will crash
        std::cout << "C: " << c2->get() << " @ " << &*c2 << std::endl;
    }
}

