#include <vector>
#include <memory>
#include <iostream>

struct Widget : public std::vector<int> {
    Widget(std::initializer_list<int> l) : std::vector<int>(l) {
        std::cout << "New Widget constructed at " << this << std::endl;
    }
    Widget(Widget&& w) : std::vector<int>(std::move(w)) {
        std::cout << "New Widget constructed at " << this << ", moved from Widget at " << &w << std::endl;
    }
    ~Widget() { std::cout << "Deleting Widget at " << this << std::endl; }
};

void Consume(Widget&& w) {
    Widget ww(std::move(w));
    for (auto& x : ww) {
        x *= 2;
    }
    std::cout << "Widget consumed: " << ww.size() << " ";
    for (auto x : ww) std::cout << x << " ";
    std::cout << "\nDeleting new widget at " << &ww << std::endl;
};

int main() {
    Widget w{{1, 2, 3}};
    std::cout << "Initial w: " << w.size() << " ";
    for (auto x : w) std::cout << x << " ";
    std::cout << std::endl;
    std::cout << "Calling consume" << std::endl;
    Consume(std::move(w));
    std::cout << "Deleting moved-from widget at " << &w << std::endl;
}
