// Version 26 with policy rebind
#include <cstdlib>
#include <utility>
#include <cassert>
#include <iostream>

template <typename T>
struct DeleteByOperator {
    void operator()(T* p) const { 
        delete p;
    }
    template <typename U> using rebind = DeleteByOperator<U>;
};

template <typename T>
struct DeleteByFree {
    void operator()(T* p) const {
        p->~T();
        free(p);
    }
    template <typename U> using rebind = DeleteByFree<U>;
};

template <typename T>
struct DeleteDestructorOnly {
    void operator()(T* p) const {
        p->~T();
    }
    template <typename U> using rebind = DeleteDestructorOnly<U>;
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
    template <typename U> using rebind = DeleteSmallHeap<U>;
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

template <typename P, typename T>
struct NoRaw {
};

template <typename P, typename T>
struct ExplicitRaw {
    explicit operator T*() { return static_cast<P*>(this)->p_; }
    explicit operator const T*() const { return static_cast<const P*>(this)->p_; }
};

template <typename P, typename T>
struct ImplicitRaw {
    operator T*() { return static_cast<P*>(this)->p_; }
    operator const T*() const { return static_cast<const P*>(this)->p_; }
};

template <typename T, typename DeletionPolicy = DeleteByOperator<T>,
                      typename CopyMovePolicy = NoMoveNoCopy,
                      template <typename, typename> class ConversionPolicy = ExplicitRaw>
class SmartPtr : private DeletionPolicy, public CopyMovePolicy,
                 public ConversionPolicy<SmartPtr<T, DeletionPolicy, CopyMovePolicy, ConversionPolicy>, T> {
    T* p_;
    template<typename, typename> friend class ConversionPolicy;
    public:
    using value_t = T;
    using deletion_policy_t = DeletionPolicy;
    using copy_move_policy_t = CopyMovePolicy;
    template <typename P, typename T1> using conversion_policy_t = ConversionPolicy<P, T1>;
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy())
        : DeletionPolicy(std::move(deletion_policy)), p_(p)
    {}
    ~SmartPtr() { 
        if (CopyMovePolicy::must_delete()) DeletionPolicy::operator()(p_);
    }
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

    template <typename U> using rebind = SmartPtr<U, typename DeletionPolicy::template rebind<U>, CopyMovePolicy, ConversionPolicy>;
};

class C {
    int i_ {};
    public:
    explicit C(int i) : i_(i) { std::cout << "Constructing C(" << i << ") @" << this << std::endl; }
    ~C() { std::cout << "Destroying C @" << this << std::endl; }
    int get() const { return i_; }
};

class D {
    double x_ {};
    public:
    explicit D(double x) : x_(x) { std::cout << "Constructing D(" << x << ") @" << this << std::endl; }
    ~D() { std::cout << "Destroying D @" << this << std::endl; }
    double get() const { return x_; }
};

int main() {
    {
        SmartPtr<C, DeleteByOperator<C>, MoveNoCopy, ExplicitRaw> c(new C(42));
        std::cout << "C: " << c->get() << " @ " << &*c << std::endl;

        using dptr_t = decltype(c)::rebind<D>;
        dptr_t d(new D(4.2));
        D* p1(d);       // OK
        //D* p2 = d;      // Does not compile - ExplicitRaw rebound from c
        std::cout << "D: " << d->get() << " " << p1->get() << std::endl;
    }
}

