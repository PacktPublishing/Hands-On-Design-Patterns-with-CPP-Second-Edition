// Version 11 with concept-based release policy
#include <cstdlib>
#include <utility>
#include <cassert>
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

struct WithRelease {
    static constexpr bool enabled = true;
};

struct NoRelease {
    static constexpr bool enabled = false;
};

template <typename T, typename DeletionPolicy = DeleteByOperator<T>, typename ReleasePolicy = WithRelease>
class SmartPtr : private DeletionPolicy, public ReleasePolicy
{
    T* p_;
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy())
        : DeletionPolicy(std::move(deletion_policy)), p_(p)
    {}
    ~SmartPtr() { 
        if (p_) DeletionPolicy::operator()(p_);
    }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
    SmartPtr(SmartPtr&& that) :
        DeletionPolicy(std::move(that)),
        p_(std::exchange(that.p_, nullptr))
    {}
    SmartPtr& operator=(SmartPtr&& that) {
        DeletionPolicy::operator()(p_);
        p_ = std::exchange(that.p_, nullptr);
        static_cast<DeletionPolicy&>(*this) = std::move(that);
        return *this;
    }
    void release() requires ReleasePolicy::enabled { p_ = nullptr; }
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
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
        C* p = &*c;
        c.release();            // No automatic deletion
        delete p;               // Manual deletion
    }

    {
        SmallHeap h;
        SmartPtr<C, DeleteSmallHeap<C>, NoRelease> c{new(&h) C(42), DeleteSmallHeap<C>(h)};
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
        //c.release();    // Does not compile
    }
    {
        SmartPtr<C> c(new C(42));
        SmartPtr<C> c1(std::move(c));
    }
}

