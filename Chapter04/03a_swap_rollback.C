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
    vec v1{C(1), C(2), C(3), C(4)};
    {   // No exceptions - OK
        bad_i = -1;
        vec v2;
        transmogrify(v1, v2);
        std::cout << "v2(" << v2.size() << "/" << v2.capacity() << "):"; for (auto x: v2) std::cout << " " << x.i_; std::cout << std::endl;
    }
    {   // exception - partially constructed result
        bad_i = 6;
        vec v2;
        try { transmogrify(v1, v2); } catch (...) {}
        std::cout << "v2(" << v2.size() << "/" << v2.capacity() << "):"; for (auto x: v2) std::cout << " " << x.i_; std::cout << std::endl;
    }
    {   // exception - initial content lost
        bad_i = 1;
        vec v2{C(5)};
        try { transmogrify(v1, v2); } catch (...) {}
        std::cout << "v2(" << v2.size() << "/" << v2.capacity() << "):"; for (auto x: v2) std::cout << " " << x.i_; std::cout << std::endl;
    }
}
