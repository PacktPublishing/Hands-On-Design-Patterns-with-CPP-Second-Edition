#include <vector>
#include <iostream>

template <typename T> class registry {
    public:
    static size_t count;
    static T* head;
    T* prev;
    T* next;
    protected:
    registry() {
        ++count;
        prev = nullptr;
        next = head;
        head = static_cast<T*>(this);
        if (next) next->prev = head;
    }
    registry(const registry&) {
        ++count;
        prev = nullptr;
        next = head;
        head = static_cast<T*>(this);
        if (next) next->prev = head;
    }
    ~registry() {
        --count;
        if (prev) prev->next = next;
        if (next) next->prev = prev;
        if (head == this) head = next;
    }

};
template <typename T> size_t registry<T>::count(0);
template <typename T> T* registry<T>::head(nullptr);


class A : public registry<A> {
    int i_;
    public:
    A(int i) : i_(i) {}
    friend std::ostream& operator<<(std::ostream& out, const A& a) { out << a.i_; return out; }
};

class B : public registry<B> {
    int i_;
    public:
    B(int i) : i_(i) {}
    friend std::ostream& operator<<(std::ostream& out, const B& b) { out << b.i_; return out; }
};

template <typename T> void report() {
    std::cout << "Count: " << T::count; for (const T* p = T::head; p; p = p->next) std::cout << " " << *p; std::cout << std::endl;
}

int main() {
    {
        A a(0);
        std::cout << A::count << " " << B::count << std::endl;
    }
    std::cout << A::count << " " << B::count << std::endl;
    {
        B b1(1);
        B b2 = std::move(b1);
        std::cout << A::count << " " << B::count << std::endl;
    }
    std::cout << A::count << " " << B::count << std::endl;
    {
        std::vector<A> v { 1, 2, 3, 4 };
        std::cout << A::count << " " << B::count << std::endl;
    }
    std::cout << A::count << " " << B::count << std::endl;

    report<A>();
    A* a4 = nullptr;
    {
        A a1(1);
        report<A>();
        a4 = new A(4);
        A a2(2);
        B b1(10);
        report<A>();
        report<B>();
    }
    report<A>();
    A a3(3);
    report<A>();
    delete a4;
    report<A>();
    report<B>();
}
