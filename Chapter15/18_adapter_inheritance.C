// Version 14 with debug policy.
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

class NoMoveCopyRefCounted {
    size_t* count_ {};
    protected:
    NoMoveCopyRefCounted() : count_(new size_t(1)) {}
    NoMoveCopyRefCounted(const NoMoveCopyRefCounted& that)
        : count_(that.count_)
    {
        ++(*count_);
    }
    NoMoveCopyRefCounted(NoMoveCopyRefCounted&&) = delete;
    ~NoMoveCopyRefCounted() {
        --(*count_);
        if (*count_ == 0) {
            delete count_;
        }
    }
    NoMoveCopyRefCounted& operator=(const NoMoveCopyRefCounted&& that) {
        --(*count_);
        if (*count_ == 0) {
            delete count_;
        }
        count_ = that.count_;
        ++(*count_);
        return *this;
    }
    NoMoveCopyRefCounted& operator=(NoMoveCopyRefCounted&&) = delete;
    bool must_delete() const { return *count_ == 1; }
    public:
    size_t count() const { return *count_; }
};

struct NoDebug {
    template <typename P, typename T> static void construct(const P* ptr, const T* p) {}
    template <typename P, typename T> static void move(const P* ptr, const P* from, const T* p) {}
    template <typename P, typename T> static void copy(const P* ptr, const P* from, const T* p) {}
    template <typename P, typename T> static void destroy(const P* ptr, const T* p, bool must_delete) {}
};

struct Debug {
    template <typename P, typename T> static void construct(const P* ptr, const T* p) {
        std::cout << "Constructed SmartPtr at " << ptr << ", object " << static_cast<const void*>(p) << std::endl;
    }
    template <typename P, typename T> static void move(const P* ptr, const P* from, const T* p) {
        std::cout << "Moved SmartPtr at " << ptr << " from " << from << ", object " << static_cast<const void*>(p) << std::endl;
    }
    template <typename P, typename T> static void copy(const P* ptr, const P* from, const T* p) {
        std::cout << "Copied SmartPtr at " << ptr << " from " << from << ", object " << static_cast<const void*>(p) << std::endl;
    }
    template <typename P, typename T> static void destroy(const P* ptr, const T* p, bool must_delete) {
        std::cout << "Destroyed SmartPtr at " << ptr << ", object " << static_cast<const void*>(p) << (must_delete ? " is" : " is not") << " deleted" << std::endl;
    }
};

template <typename T, typename DeletionPolicy = DeleteByOperator<T>,
                      typename CopyMovePolicy = NoMoveNoCopy,
                      typename DebugPolicy = NoDebug>
class SmartPtr : private DeletionPolicy, public CopyMovePolicy {
    T* p_;
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy())
        : DeletionPolicy(std::move(deletion_policy)), p_(p)
    {
        DebugPolicy::construct(this, p);
    }
    ~SmartPtr() { 
        DebugPolicy::destroy(this, p_, CopyMovePolicy::must_delete());
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
    {
        DebugPolicy::copy(this, &that, p_);
    }
    SmartPtr(SmartPtr&& that) :
        DeletionPolicy(std::move(that)),
        CopyMovePolicy(std::move(that)),
        p_(that.p_)
    {
        DebugPolicy::move(this, &that, p_);
        that.p_ = nullptr;
    }
};

template <typename T,
          typename DebugPolicy = NoDebug
         >
class SmartPtrAdapter : public SmartPtr<T, DeleteByOperator<T>, NoMoveCopyRefCounted, DebugPolicy>
{
    using base_t = SmartPtr<T, DeleteByOperator<T>, NoMoveCopyRefCounted, DebugPolicy>;
    using base_t::SmartPtr;
    using base_t::operator=;
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
        SmartPtrAdapter<C, Debug> c{new C(42)};
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
    }
}

