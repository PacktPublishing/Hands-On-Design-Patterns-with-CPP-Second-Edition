class C;
C operator+(const C& lhs, const C& rhs);
class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
    friend C operator+(const C& lhs, const C& rhs) { return C(lhs.x_ + rhs.x_); }
};

C f(const C& x, const C& y) {
    return x + y;
}

