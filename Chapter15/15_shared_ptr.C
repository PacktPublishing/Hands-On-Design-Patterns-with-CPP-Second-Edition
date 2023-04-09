// Version 14 with copy/move policy.
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
    DeleteSmallHeap(const DeleteSmallHeap&) = default;
    DeleteSmallHeap& operator=(const DeleteSmallHeap&) = default;
    private:
    SmallHeap* heap_;
};

class NoMoveNoCopy {
    protected:
    NoMoveNoCopy() = default;
    NoMoveNoCopy(NoMoveNoCopy&&) = delete;
    NoMoveNoCopy(const NoMoveNoCopy&) = delete;
    constexpr bool must_delete() const { return true; }
};

class MoveNoCopy {
    protected:
    MoveNoCopy() = default;
    MoveNoCopy(MoveNoCopy&&) = default;
    MoveNoCopy(const MoveNoCopy&) = delete;
    constexpr bool must_delete() const { return true; }
};

class MoveCopyRefCounted {
    size_t* count_ {};
    protected:
    MoveCopyRefCounted() : count_(new size_t(1)) {}
    MoveCopyRefCounted(const MoveCopyRefCounted& that)
        : count_(that.count_)
    {
        if (count_) ++(*count_);
    }
    MoveCopyRefCounted(MoveCopyRefCounted&& that) :
        count_(std::exchange(that.count_, nullptr))
    {}
    ~MoveCopyRefCounted() {
        if (!count_) return;
        --(*count_);
        if (*count_ == 0) {
            delete count_;
        }
    }
    MoveCopyRefCounted& operator=(const MoveCopyRefCounted&& that) {
        if (count_) {
            --(*count_);
            if (*count_ == 0) {
                delete count_;
            }
        }
        count_ = that.count_;
        if (count_) ++(*count_);
        return *this;
    }
    MoveCopyRefCounted& operator=(MoveCopyRefCounted&& that) {
        if (count_) {
            --(*count_);
            if (*count_ == 0) {
                delete count_;
            }
        }
        count_ = std::exchange(that.count_, nullptr);
        return *this;
    }
    bool must_delete() const { return count_ && *count_ == 1; }
    public:
    size_t count() const { return *count_; }
};

template <typename T, typename DeletionPolicy = DeleteByOperator<T>,
                      typename CopyMovePolicy = NoMoveNoCopy>
class SmartPtr : private DeletionPolicy, public CopyMovePolicy {
    T* p_;
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy())
        : DeletionPolicy(std::move(deletion_policy)), p_(p)
    {}
    ~SmartPtr() { 
        if (CopyMovePolicy::must_delete()) DeletionPolicy::operator()(p_);
    }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    SmartPtr(const SmartPtr& that) :
        DeletionPolicy(that),
        CopyMovePolicy(that),
        p_(that.p_)
    {}
    SmartPtr& operator=(const SmartPtr& that) {
        if (this == &that) return *this;
        if (CopyMovePolicy::must_delete()) DeletionPolicy::operator()(p_);
        p_ = that.p_;
        DeletionPolicy::operator=(that);
        CopyMovePolicy::operator=(that);
        return *this;
    }
    SmartPtr(SmartPtr&& that) :
        DeletionPolicy(std::move(that)),
        CopyMovePolicy(std::move(that)),
        p_(std::exchange(that.p_, nullptr))
    {}
    SmartPtr& operator=(SmartPtr&& that) {
        if (this == &that) return *this;
        if (CopyMovePolicy::must_delete()) DeletionPolicy::operator()(p_);
        p_ = std::exchange(that.p_, nullptr);
        DeletionPolicy::operator=(std::move(that));
        CopyMovePolicy::operator=(std::move(that));
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
        SmartPtr<C, DeleteByOperator<C>, MoveCopyRefCounted> c{new C(42)};
        std::cout << "C: " << c->get() << " @ " << &*c << " n=" << c.count() << std::endl;
        auto c1(c);
        std::cout << "C: " << c->get() << " @ " << &*c << " n=" << c.count() << std::endl;
        std::cout << "C: " << c1->get() << " @ " << &*c1 << " n=" << c1.count() << std::endl;
        {
            auto c2(c);
            std::cout << "C: " << c2->get() << " @ " << &*c2 << " n=" << c2.count() << std::endl;
        }
        std::cout << "C: " << c1->get() << " @ " << &*c1 << " n=" << c1.count() << std::endl;
        std::cout << "C: " << c->get() << " @ " << &*c << " n=" << c.count() << std::endl;
        auto c2(std::move(c1));
        std::cout << "C: " << c2->get() << " @ " << &*c2 << " n=" << c2.count() << std::endl;
        std::cout << "C: " << c->get() << " @ " << &*c << " n=" << c.count() << std::endl;
        c1 = std::move(c);
        std::cout << "C: " << c1->get() << " @ " << &*c1 << " n=" << c1.count() << std::endl;
        std::cout << "C: " << c2->get() << " @ " << &*c2 << " n=" << c2.count() << std::endl;
        //(void)c.count();      // Crashes - moved-from object
    }

    {
        SmartPtr<C, DeleteByOperator<C>, MoveNoCopy> c{new C(42)};
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
        auto c1(std::move(c));
        std::cout << "C: " << c1->get() << " @ " << &*c1 << std::endl;
        //auto c2(c);   // Does not compile
    }
}

