#include <iostream>
#include <utility>
#include <sstream>
#include <string>

template <typename D> struct Factory {
    template <typename... Args> static D* create(Args&&... args) { return new D(std::forward<Args>(args)...); }
    static void destroy(D* d) { delete d; }
};

template <typename D> struct Stringify {
    operator std::string() const {
        std::stringstream S;
        S << *static_cast<const D*>(this);
        return S.str();
    }
};

template <typename D> struct CountedObject {
    static size_t count;
    CountedObject() { ++count; }
    CountedObject(const CountedObject&) { ++count; }
    CountedObject(CountedObject&&) { ++count; }
    ~CountedObject() { --count; }
};
template <typename D> size_t CountedObject<D>::count = 0;

class C1 : public Factory<C1>, public Stringify<C1> {
    int i_;
    public:
    explicit C1(int i) : i_(i) {}
    int get() const { return i_; }
    friend std::ostream& operator<<(std::ostream& out, const C1 c1) { out << c1.i_; return out; }
};

class C2 : public Factory<C2>, public CountedObject<C2> {
    double x_;
    public:
    int get() const { return x_; }
    explicit C2(double x) : x_(x) {}
    friend std::ostream& operator<<(std::ostream& out, const C2 c2) { out << c2.x_; return out; }
};

int main() {
    C1 c1(42);
    std::string s1(c1);
    std::cout << s1 << std::endl;

    C2* c2 = C2::create(3.14);
    //std::string s2(c2); // Does not compile - not using Stringify policy
    std::cout << *c2 << std::endl;
    std::cout << "C2 count " << C2::count << std::endl;
    C2::destroy(c2);
    std::cout << "C2 count " << C2::count << std::endl;
}
