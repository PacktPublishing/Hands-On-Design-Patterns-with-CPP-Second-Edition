#include <iostream>
#include <cmath>

class Point {
    double x_ {};
    double y_ {};
    Point(double x, double y) : x_(x), y_(y) {}

    public:
    Point() = default;
    static Point make_cartesian(double x, double y) { return Point(x, y); }
    static Point make_polar(double r, double phi) { return Point(r*std::cos(phi), r*std::sin(phi)); }
    friend std::ostream& operator<<(std::ostream& out, const Point& p) { out << "(" << p.x_ << ", " << p.y_ << ")"; return out; }
};

int main() {
    Point p1(Point::make_cartesian(3, 4));
    Point p2(Point::make_polar(5, 0.927295));
    std::cout << p1 << " - " << p2 << std::endl;
}
