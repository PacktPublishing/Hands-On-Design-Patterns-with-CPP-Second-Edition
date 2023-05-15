// Value type decorators
#include <iostream>
using std::cout;
using std::endl;

template <typename V> class Value {
    public:
    template <typename FV1> using rebind = Value<V>;
    using basic_type = V;
    using value_type = Value;
    explicit Value() = default;
    explicit Value(basic_type rhs) : val_(rhs) {}
    Value(const Value& rhs) : val_(rhs.val_) {}
    Value& operator=(const Value& rhs) { val_ = rhs.val_; return *this; }
    Value& operator=(basic_type rhs) { val_ = rhs; return *this; }
    friend std::ostream& operator<<(std::ostream& out, Value x) { out << x.val_; return out; }
    friend std::istream& operator>>(std::istream& in, Value& x) { in >> x.val_; return in; }

    protected:
    V val_ {};
};

template <typename V, typename FV = void> class ImplicitFrom : public V::template rebind<FV> {
    using base_t = typename V::template rebind<FV>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = ImplicitFrom<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit ImplicitFrom(value_type v) : base_t(v) {}
    ImplicitFrom(basic_type rhs) : base_t(rhs) {}
};

template <typename V> class ImplicitFrom<V, void> : public V::template rebind<ImplicitFrom<V>> {
    using base_t = typename V::template rebind<ImplicitFrom>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = ImplicitFrom<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit ImplicitFrom(value_type v) : base_t(v) {}
    ImplicitFrom(basic_type rhs) : base_t(rhs) {}
};

template <typename V, typename FV = void> class ImplicitTo : public V::template rebind<FV> {
    using base_t = typename V::template rebind<FV>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = ImplicitTo<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit ImplicitTo(value_type v) : base_t(v) {}
    operator basic_type() { return this->val_; }
    operator const basic_type() const { return this->val_; }
};

template <typename V> class ImplicitTo<V, void> : public V::template rebind<ImplicitTo<V>> {
    using base_t = typename V::template rebind<ImplicitTo>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = ImplicitTo<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit ImplicitTo(value_type v) : base_t(v) {}
    operator basic_type() { return this->val_; }
    operator const basic_type() const { return this->val_; }
};

template <typename V, typename FV = void> class Comparable : public V::template rebind<FV> {
    using base_t = typename V::template rebind<FV>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = Comparable<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit Comparable(value_type v) : base_t(v) {}
    friend bool operator==(FV lhs, FV rhs) { return lhs.val_ == rhs.val_; }
    friend bool operator!=(FV lhs, FV rhs) { return lhs.val_ != rhs.val_; }
    friend bool operator==(FV lhs, basic_type rhs) { return lhs.val_ == rhs; }
    friend bool operator==(basic_type lhs, FV rhs) { return lhs == rhs.val_; }
    friend bool operator!=(FV lhs, basic_type rhs) { return lhs.val_ != rhs; }
    friend bool operator!=(basic_type lhs, FV rhs) { return lhs != rhs.val_; }
};

template <typename V> class Comparable<V, void> : public V::template rebind<Comparable<V>> {
    using base_t = typename V::template rebind<Comparable>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = Comparable<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit Comparable(value_type v) : base_t(v) {}
    friend bool operator==(Comparable lhs, Comparable rhs) { return lhs.val_ == rhs.val_; }
    friend bool operator!=(Comparable lhs, Comparable rhs) { return lhs.val_ != rhs.val_; }
    friend bool operator==(Comparable lhs, basic_type rhs) { return lhs.val_ == rhs; }
    friend bool operator==(basic_type lhs, Comparable rhs) { return lhs == rhs.val_; }
    friend bool operator!=(Comparable lhs, basic_type rhs) { return lhs.val_ != rhs; }
    friend bool operator!=(basic_type lhs, Comparable rhs) { return lhs != rhs.val_; }
};

template <typename V, typename FV = void> class Ordered : public V::template rebind<FV> {
    using base_t = typename V::template rebind<FV>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = Ordered<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit Ordered(value_type v) : base_t(v) {}
    friend bool operator<(FV lhs, FV rhs) { return lhs.val_ < rhs.val_; }
    friend bool operator<(basic_type lhs, FV rhs) { return lhs < rhs.val_; }
    friend bool operator<(FV lhs, basic_type rhs) { return lhs.val_ < rhs; }
    friend bool operator<=(FV lhs, FV rhs) { return lhs.val_ <= rhs.val_; }
    friend bool operator<=(basic_type lhs, FV rhs) { return lhs <= rhs.val_; }
    friend bool operator<=(FV lhs, basic_type rhs) { return lhs.val_ <= rhs; }
    friend bool operator>(FV lhs, FV rhs) { return lhs.val_ > rhs.val_; }
    friend bool operator>(basic_type lhs, FV rhs) { return lhs > rhs.val_; }
    friend bool operator>(FV lhs, basic_type rhs) { return lhs.val_ > rhs; }
    friend bool operator>=(FV lhs, FV rhs) { return lhs.val_ >= rhs.val_; }
    friend bool operator>=(basic_type lhs, FV rhs) { return lhs >= rhs.val_; }
    friend bool operator>=(FV lhs, basic_type rhs) { return lhs.val_ >= rhs; }
};

template <typename V> class Ordered<V, void> : public V::template rebind<Ordered<V>> {
    using base_t = typename V::template rebind<Ordered>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = Ordered<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit Ordered(value_type v) : base_t(v) {}
    friend bool operator<(Ordered lhs, Ordered rhs) { return lhs.val_ < rhs.val_; }
    friend bool operator<(basic_type lhs, Ordered rhs) { return lhs < rhs.val_; }
    friend bool operator<(Ordered lhs, basic_type rhs) { return lhs.val_ < rhs; }
    friend bool operator<=(Ordered lhs, Ordered rhs) { return lhs.val_ <= rhs.val_; }
    friend bool operator<=(basic_type lhs, Ordered rhs) { return lhs <= rhs.val_; }
    friend bool operator<=(Ordered lhs, basic_type rhs) { return lhs.val_ <= rhs; }
    friend bool operator>(Ordered lhs, Ordered rhs) { return lhs.val_ > rhs.val_; }
    friend bool operator>(basic_type lhs, Ordered rhs) { return lhs > rhs.val_; }
    friend bool operator>(Ordered lhs, basic_type rhs) { return lhs.val_ > rhs; }
    friend bool operator>=(Ordered lhs, Ordered rhs) { return lhs.val_ >= rhs.val_; }
    friend bool operator>=(basic_type lhs, Ordered rhs) { return lhs >= rhs.val_; }
    friend bool operator>=(Ordered lhs, basic_type rhs) { return lhs.val_ >= rhs; }
};

template <typename V, typename FV = void> class Addable : public V::template rebind<FV> {
    using base_t = typename V::template rebind<FV>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = Addable<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit Addable(value_type v) : base_t(v) {}
    friend FV operator+(FV lhs, FV rhs) { return FV(lhs.val_ + rhs.val_); }
    friend FV operator+(FV lhs, basic_type rhs) { return FV(lhs.val_ + rhs); }
    friend FV operator+(basic_type lhs, FV rhs) { return FV(lhs + rhs.val_); }
    friend FV operator-(FV lhs, FV rhs) { return FV(lhs.val_ - rhs.val_); }
    friend FV operator-(FV lhs, basic_type rhs) { return FV(lhs.val_ - rhs); }
    friend FV operator-(basic_type lhs, FV rhs) { return FV(lhs - rhs.val_); }
};

template <typename V> class Addable<V, void> : public V::template rebind<Addable<V>> {
    using base_t = typename V::template rebind<Addable>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = Addable<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit Addable(value_type v) : base_t(v) {}
    friend Addable operator+(Addable lhs, Addable rhs) { return Addable(lhs.val_ + rhs.val_); }
    friend Addable operator+(Addable lhs, basic_type rhs) { return Addable(lhs.val_ + rhs); }
    friend Addable operator+(basic_type lhs, Addable rhs) { return Addable(lhs + rhs.val_); }
    friend Addable operator-(Addable lhs, Addable rhs) { return Addable(lhs.val_ - rhs.val_); }
    friend Addable operator-(Addable lhs, basic_type rhs) { return Addable(lhs.val_ - rhs); }
    friend Addable operator-(basic_type lhs, Addable rhs) { return Addable(lhs - rhs.val_); }
};

template <typename V, typename FV = void> class Multipliable : public V::template rebind<FV> {
    using base_t = typename V::template rebind<FV>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = Multipliable<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit Multipliable(value_type v) : base_t(v) {}
    friend FV operator*(FV lhs, FV rhs) { return FV(lhs.val_ * rhs.val_); }
    friend FV operator*(FV lhs, basic_type rhs) { return FV(lhs.val_ * rhs); }
    friend FV operator*(basic_type lhs, FV rhs) { return FV(lhs * rhs.val_); }
    friend FV operator/(FV lhs, FV rhs) { return FV(lhs.val_ / rhs.val_); }
    friend FV operator/(FV lhs, basic_type rhs) { return FV(lhs.val_ / rhs); }
    friend FV operator/(basic_type lhs, FV rhs) { return FV(lhs / rhs.val_); }
};

template <typename V> class Multipliable<V, void> : public V::template rebind<Multipliable<V>> {
    using base_t = typename V::template rebind<Multipliable>;
    public:
    using base_t::base_t;
    using base_t::operator=;
    template <typename FV1> using rebind = Multipliable<V, FV1>;
    using value_type = typename base_t::value_type;
    using basic_type = typename value_type::basic_type;
    explicit Multipliable(value_type v) : base_t(v) {}
    friend Multipliable operator*(Multipliable lhs, Multipliable rhs) { return Multipliable(lhs.val_ * rhs.val_); }
    friend Multipliable operator*(Multipliable lhs, basic_type rhs) { return Multipliable(lhs.val_ * rhs); }
    friend Multipliable operator*(basic_type lhs, Multipliable rhs) { return Multipliable(lhs * rhs.val_); }
    friend Multipliable operator/(Multipliable lhs, Multipliable rhs) { return Multipliable(lhs.val_ / rhs.val_); }
    friend Multipliable operator/(Multipliable lhs, basic_type rhs) { return Multipliable(lhs.val_ / rhs); }
    friend Multipliable operator/(basic_type lhs, Multipliable rhs) { return Multipliable(lhs / rhs.val_); }
};

int main() {
    {
        using V = Value<int>;
        V i, j(5), k(3);
        i = j; j = 1;
#ifdef ERROR1
        i == j; // Not Comparable
#endif // ERROR1 
    }
    {
        using V = Comparable<Value<int>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
#ifdef ERROR2
        i < j; // Not Ordered
#endif // ERROR2 
    }
    {
        using V = Ordered<Comparable<Value<int>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
#ifdef ERROR3
        i + j; // Not Addable
#endif // ERROR3
    }
    {
        using V = Comparable<Ordered<Value<int>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
#ifdef ERROR4
        i + j; // Not Addable
#endif // ERROR4
    }
    {
        using V = Addable<Comparable<Value<int>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i + j == k: " << ((i + j) == k) << ", i - j != k: " << ((i - j) != k) << endl;
        cout << "i + j == k + 1: " << ((i + j) == (k + 1)) << ", i - j != k + 1: " << ((i - j) != (k + 1)) << endl; // No order problem
        static_assert(std::is_same<V, std::decay_t<decltype(i + j)>>::value, "");
#ifdef ERROR5
        i*j; // Not Multipliable
#endif // ERROR5
    }
    {
        using V = Comparable<Addable<Value<int>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i + j == k: " << ((i + j) == k) << ", i - j != k: " << ((i - j) != k) << endl;
        cout << "i + j == k + 1: " << ((i + j) == (k + 1)) << ", i - j != k + 1: " << ((i - j) != (k + 1)) << endl; // No order problem
        static_assert(std::is_same<V, std::decay_t<decltype(i + j)>>::value, "");
#ifdef ERROR6
        i*j; // Not Multipliable
#endif // ERROR6
    }
    {
        using V = Addable<Comparable<Ordered<Value<int>>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i + j > k: " << ((i + j) > k) << ", i - j != k: " << ((i - j) != k) << endl;
        cout << "i + j > k + 1: " << ((i + j) > (k + 1)) << ", i - j != k + 1: " << ((i - j) != (k + 1)) << endl; // No order problem
        static_assert(std::is_same<V, std::decay_t<decltype(i + j)>>::value, "");
#ifdef ERROR7
        i*j; // Not Multipliable
#endif // ERROR7
    }
    {
        using V = Comparable<Addable<Ordered<Value<int>>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i + j > k: " << ((i + j) > k) << ", i - j != k: " << ((i - j) != k) << endl;
        cout << "i + j > k + 1: " << ((i + j) > (k + 1)) << ", i - j != k + 1: " << ((i - j) != (k + 1)) << endl; // No order problem
        static_assert(std::is_same<V, std::decay_t<decltype(i + j)>>::value, "");
#ifdef ERROR8
        i*j; // Not Multipliable
#endif // ERROR8
    }
    {
        using V = Comparable<Ordered<Addable<Value<int>>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i + j > k: " << ((i + j) > k) << ", i - j != k: " << ((i - j) != k) << endl;
        cout << "i + j > k + 1: " << ((i + j) > (k + 1)) << ", i - j != k + 1: " << ((i - j) != (k + 1)) << endl; // No order problem
#ifdef ERROR9
        i*j; // Not Multipliable
#endif // ERROR9
    }
    {
        using V = Multipliable<Addable<Value<int>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i * j: " << (i * j) << ", i / k: " << (i / k) << endl;
        cout << "i * 3: " << (i * 3) << ", 7 / k: " << (7 / k) << endl;
        cout << "(i * j) + (k / 3): " << ((i * j) + (k / 3)) << endl;
        cout << "(i + j) * (k + 3): " << ((i + j) * (k + 3)) << endl; // No order problem
    }
    {
        using V = Addable<Multipliable<Value<int>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i * j: " << (i * j) << ", i / k: " << (i / k) << endl;
        cout << "i * 3: " << (i * 3) << ", 7 / k: " << (7 / k) << endl;
        cout << "(i * j) + (k / 3): " << ((i * j) + (k / 3)) << endl; // No order problem
        cout << "(i + j) * (k + 3): " << ((i + j) * (k + 3)) << endl;
    }
    {
        using V = Comparable<Multipliable<Addable<Value<int>>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i * j: " << (i * j) << ", i / k: " << (i / k) << endl;
        cout << "i * 3: " << (i * 3) << ", 7 / k: " << (7 / k) << endl;
        cout << "(i * j) + (k / 3): " << ((i * j) + (k / 3)) << endl;
        cout << "(i + j) * (k + 3): " << ((i + j) * (k + 3)) << endl; // No order problem
    }
    {
        using V = Comparable<Addable<Multipliable<Value<int>>>>;
        V i, j(5), k(3);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i * j: " << (i * j) << ", i / k: " << (i / k) << endl;
        cout << "i * 3: " << (i * 3) << ", 7 / k: " << (7 / k) << endl;
        cout << "(i * j) + (k / 3): " << ((i * j) + (k / 3)) << endl; // No order problem
        cout << "(i + j) * (k + 3): " << ((i + j) * (k + 3)) << endl;
    }
#ifdef ERROR10
    {
        using V = Value<int>;
        V i = 3;
        auto f = [](V i) {};
        f(5);
    }
    {
        using V = Comparable<Addable<Multipliable<Value<int>>>>;
        V i = 3;
        auto f = [](V i) {};
        f(5);
    }
#endif // ERROR10
    {
        using V = ImplicitFrom<Value<int>>;
        V i = 3;
        auto f = [](V i) {};
        f(5);
    }
    {
        using V = ImplicitFrom<Comparable<Addable<Multipliable<Value<int>>>>>;
        V i = 3, j(5), k(3);
        auto f = [](V i) {};
        f(5);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i * j: " << (i * j) << ", i / k: " << (i / k) << endl;
        cout << "i * 3: " << (i * 3) << ", 7 / k: " << (7 / k) << endl;
        cout << "(i * j) + (k / 3): " << ((i * j) + (k / 3)) << endl; // No order problem
        cout << "(i + j) * (k + 3): " << ((i + j) * (k + 3)) << endl;
    }
    {
        using V = Comparable<Addable<ImplicitFrom<Multipliable<Value<int>>>>>;
        V i = 3, j(5), k(3);
        auto f = [](V i) {};
        f(5);
        i = j; j = 1;
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i * j: " << (i * j) << ", i / k: " << (i / k) << endl;
        cout << "i * 3: " << (i * 3) << ", 7 / k: " << (7 / k) << endl;
        cout << "(i * j) + (k / 3): " << ((i * j) + (k / 3)) << endl; // No order problem
        cout << "(i + j) * (k + 3): " << ((i + j) * (k + 3)) << endl;
    }
#ifdef ERROR11
    {
        using V = Value<int>;
        V i(3);
        int j = i;
    }
    {
        using V = Comparable<Addable<Multipliable<Value<int>>>>;
        V i(3);
        int j = i;
    }
#endif // ERROR11
    {
        using V = ImplicitTo<Value<int>>;
        V i(3);
        int j = i;
    }
    {
        using V = ImplicitTo<Comparable<Addable<Multipliable<Value<int>>>>>;
        V i(3);
        int j = i;
    }
    {
        using V = Comparable<Addable<ImplicitTo<Multipliable<Value<int>>>>>;
        V i(3);
        int j = i;
    }
}
