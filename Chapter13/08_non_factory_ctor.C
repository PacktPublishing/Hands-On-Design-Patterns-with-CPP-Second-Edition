#include <iostream>
#include <cmath>

class Point {
    double x_ {};
    double y_ {};

    public:
    Point() = default;
    struct cartesian_t {} static constexpr cartesian {};
    Point(cartesian_t, double x, double y) : x_(x), y_(y) {}
    struct polar_t {} static constexpr polar {};
    Point(polar_t, double r, double phi) : Point(cartesian, r*std::cos(phi), r*std::sin(phi)) {}
    friend std::ostream& operator<<(std::ostream& out, const Point& p) { out << "(" << p.x_ << ", " << p.y_ << ")"; return out; }
};

int main() {
    Point p1(Point::cartesian, 3, 4);
    Point p2(Point::polar, 5, 0.927295);
    std::cout << p1 << " - " << p2 << std::endl;
}
