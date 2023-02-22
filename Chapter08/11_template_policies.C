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

template <typename T, template <typename> class... Policies>
class C : public Policies<C<T, Policies...>>...
{
    T t_;
    public:
    explicit C(T t) : t_(t) {}
    const T& get() const { return t_; }
    friend std::ostream& operator<<(std::ostream& out, const C c) { out << c.t_; return out; }
};
template <typename T> using C1 = C<T, Factory, Stringify>;
using C2 = C<double, Factory, CountedObject>;

int main() {
    C1<int> c1(42);
    std::string s1(c1);
    std::cout << s1 << std::endl;

    C2* c2 = C2::create(3.14);
    std::cout << *c2 << std::endl;
    std::cout << "C2 count " << C2::count << std::endl;
    C2::destroy(c2);
    std::cout << "C2 count " << C2::count << std::endl;
}
