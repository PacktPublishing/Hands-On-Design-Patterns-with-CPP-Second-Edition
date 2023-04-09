// Version 02 with static asserts
#include <cstdlib>
#include <utility>
#include <cassert>
#include <type_traits>
#include <iostream>

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
        : heap_(heap) {}
    void operator()(T* p) const {
        p->~T();
        heap_.deallocate(p);
    }
    private:
    SmallHeap& heap_;
};

template <typename T> void delete_T(T* p) { delete p; }

template <typename T, typename DeletionPolicy = DeleteByOperator<T>>
class SmartPtr {
    T* p_;
    DeletionPolicy deletion_policy_;
    public:
    explicit SmartPtr(T* p = nullptr,
                      const DeletionPolicy& deletion_policy = DeletionPolicy())
        : p_(p), deletion_policy_(deletion_policy)
    {}
    ~SmartPtr() { 
        deletion_policy_(p_);
    }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
    SmartPtr(SmartPtr&& that) :
        p_(std::exchange(that.p_, nullptr)),
        deletion_policy_(std::move(that.deletion_policy_))
    {}
    SmartPtr& operator=(SmartPtr&& that) {
        deletion_policy_(p_);
        p_ = std::exchange(that.p_, nullptr);
        deletion_policy_ = std::move(deletion_policy_.that);
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

using delete_C_t = void (*)(C*);
void delete_C(C* p) { delete p; }

int main() {
    {
        SmartPtr<C> c(new C(42));
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
    }

    {
        SmallHeap h;
        SmartPtr<C, DeleteSmallHeap<C>> c{new(&h) C(42), DeleteSmallHeap<C>(h)};
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
    }

    {
        SmartPtr<C, delete_C_t> c(new C(42), delete_C);
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
    }

    {
        SmartPtr<C, delete_C_t> c(new C(42), delete_T<C>);
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
    }

    {
        //SmartPtr<int, delete_C_t> c(new int(42));    // Does not compile - static assert
    }
}

