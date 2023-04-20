#if __cplusplus < 202002L
#error "This example needs C++20"
#else
// Version 13 with enable_if to control conversion policies
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

struct NoRaw {
    static constexpr bool implicit_conv = false;
    static constexpr bool explicit_conv = false;
};

struct ExplicitRaw {
    static constexpr bool implicit_conv = false;
    static constexpr bool explicit_conv = true;
};

struct ImplicitRaw {
    static constexpr bool implicit_conv = true;
    static constexpr bool explicit_conv = false;
};

template <typename T, typename DeletionPolicy = DeleteByOperator<T>,
                      typename CopyMovePolicy = NoMoveNoCopy,
                      typename ConversionPolicy = ExplicitRaw>
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
    template <typename U = T> requires ConversionPolicy::implicit_conv
    operator U*() { return p_; }
    template <typename U = T> requires ConversionPolicy::implicit_conv
    operator const U*() const { return p_; }
    template <typename U = T> requires ConversionPolicy::explicit_conv
    explicit operator U*() { return p_; }
    template <typename U = T> requires ConversionPolicy::explicit_conv
    explicit operator const U*() const { return p_; }
    void release() { p_ = NULL; }
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
        SmartPtr<C, DeleteByOperator<C>, NoMoveNoCopy, ExplicitRaw> c(new C(42));
        C* p1(c);       // OK
        //C* p2 = c;      // Does not compile
        std::cout << "C: " << c->get() << " " << p1->get() << std::endl;
    }
    {
        SmartPtr<C, DeleteByOperator<C>, NoMoveNoCopy, ImplicitRaw> c(new C(42));
        C* p1(c);       // OK
        C* p2 = c;      // OK
        std::cout << "C: " << c->get() << " " << p1->get() << " " << p2->get() << std::endl;
    }
    {
        SmartPtr<C, DeleteByOperator<C>, NoMoveNoCopy, NoRaw> c(new C(42));
        //C* p1(c);       // Does not compile
        //C* p2 = c;      // Does not compile
    }
}
#endif // __cplusplus < 202002L
