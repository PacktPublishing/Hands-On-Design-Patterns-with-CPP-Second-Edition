#include <utility>
#include <vector>
#include <iostream>

int bad_i = -1;

class C {
    public:
    explicit C() : i_() {}
    explicit C(int i) : i_(i) {}
    C(const C& rhs) : i_(rhs.i_) { if (i_ == bad_i) throw i_; }
    int i_;
};

typedef std::vector<C> vec;

C transmogrify(C x) { return C(2*x.i_); }

void transmogrify(const vec& in, vec& out) {
    out.resize(0);
    out.reserve(in.size());
    for (const auto& x : in) {
        out.push_back(transmogrify(x));
    }
}

int main() {
    vec v{C(1), C(2), C(3), C(4)};
    std::cout << "Initial v(" << v.size() << "/" << v.capacity() << "):"; for (auto x: v) std::cout << " " << x.i_; std::cout << std::endl;
    {
        vec tmp;
        transmogrify(v, tmp);
        v.swap(tmp);
    }
    std::cout << "Transmogrified once v(" << v.size() << "/" << v.capacity() << "):"; for (auto x: v) std::cout << " " << x.i_; std::cout << std::endl;
    {
        vec tmp;
        transmogrify(v, tmp);
        v.swap(tmp);
    }
    std::cout << "Transmogrified twice v(" << v.size() << "/" << v.capacity() << "):"; for (auto x: v) std::cout << " " << x.i_; std::cout << std::endl;
    try { // Exception, nothing is changed
        bad_i = 12;
        vec tmp;
        transmogrify(v, tmp);
        v.swap(tmp);
    } catch (...) {}
    bad_i = -1;
    std::cout << "Transmogrified trice v(" << v.size() << "/" << v.capacity() << "):"; for (auto x: v) std::cout << " " << x.i_; std::cout << std::endl;
}
