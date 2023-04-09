// Basic smart pointer with automatic deletion.
#include <cstdlib>
#include <utility>
#include <iostream>

template <typename T>
class SmartPtr {
    T* p_;
    public:
    explicit SmartPtr(T* p = nullptr)
        : p_(p) {}
    ~SmartPtr() { 
        delete p_;
    }
    void release() { p_ = nullptr; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
    SmartPtr(SmartPtr&& that) : p_(std::exchange(that.p_, nullptr)) {}
    SmartPtr& operator=(SmartPtr&& that) { delete p_; p_ = std::exchange(that.p_, nullptr); }
};

class C {
    int i_ {};
    public:
    explicit C(int i) : i_(i) { std::cout << "Constructing C(" << i << ") @" << this << std::endl; }
    ~C() { std::cout << "Destroying C @" << this << std::endl; }
    int get() const { return i_; }
};

int main() {
    SmartPtr<C> c(new C(42));
    std::cout << "C: " << c->get() << " @ " << &*c << std::endl;
}
