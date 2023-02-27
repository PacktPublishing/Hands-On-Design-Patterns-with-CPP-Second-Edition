class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    int get() const { return x_; }
    friend C operator+(const C& lhs, const C& rhs);
};

C f(const C& x, const C& y) {
    return x + y;
}

