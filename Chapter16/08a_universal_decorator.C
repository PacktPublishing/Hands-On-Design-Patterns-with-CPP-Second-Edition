// 07 with support for void
#include <iostream>
#include <string>
using std::cout;
using std::endl;

template <typename Callable> class DebugDecorator {
    public:
    template <typename F>
    DebugDecorator(F&& f, const char* s) : c_(std::forward<F>(f)), s_(s) {}
    template <typename... Args> auto operator()(Args&&... args) const {
        cout << "Invoking " << s_ << endl;
        using res_t = decltype(c_(std::forward<Args>(args)...));
        return this->call_impl(std::is_same<res_t, void>{}, std::forward<Args>(args)...);
    }

    private:
    Callable c_;
    const std::string s_;
    template <typename... Args> auto call_impl(std::false_type, Args&&... args) const {
            auto res = c_(std::forward<Args>(args)...);
            cout << "Result: " << res << endl;
            return res;
    }
    template <typename... Args> void call_impl(std::true_type, Args&&... args) const {
        c_(std::forward<Args>(args)...);
    }
};

template <typename Callable>
auto decorate_debug(Callable&& c, const char* s) {
    return DebugDecorator<Callable>(std::forward<Callable>(c), s);
}

struct R {
    int value() const { return rand(); }
};

int g(int i, int j) { return i - j; }

struct S {
    double operator()() const { return double(rand() + 1)/double(rand() + 1); }
};

double x = 0;
double& fx() { return x; }
const double& cfx() { return x; }

int main() {
    auto f1 = decorate_debug([](int i) { return i; }, "i->i");
    f1(5);

    auto f2 = decorate_debug([](int i, int j) { return i + j; }, "i+j");
    f2(5, 3);

    auto g1 = decorate_debug(g, "g()");
    g1(5, 2);

    S s;
    auto s1 = decorate_debug(s, "rand/rand");
    s1(); s1();

    R r;
    auto f0 = decorate_debug([&]() { return r.value(); }, "rand");
    f0(); f0();

    auto fx1 = decorate_debug(fx, "fx()");
    fx() = 5; cout << "x=" << x << endl;
    //fx1() = 6; cout << "x=" << x << endl; // This is tricky but possible

    auto cfx1 = decorate_debug(cfx, "cfx()");
    cfx1();

    auto incr = decorate_debug([](int& x) { ++x; }, "++x");
    int i = 0;
    incr(i);
}
