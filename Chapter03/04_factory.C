#include <vector>
#include <memory>
#include <iostream>

struct Widget : public std::vector<int> {
    Widget(std::initializer_list<int> l) : std::vector<int>(l) {
        std::cout << "New Widget constructed" << std::endl;
    }
    ~Widget() { std::cout << "Deleting Widget" << std::endl; }
};

std::unique_ptr<Widget> WidgetFactory() {
    return std::unique_ptr<Widget>{new Widget{{ 1, 2, 3, 4, 5 }}};
}

void Double(Widget* w) {
    for (auto& x : *w) {
        x *= 2;
    }
    std::cout << "After Double(): " << w->size() << " ";
    for (auto x : *w) std::cout << x << " ";
    std::cout << std::endl;
};

int main() {
    std::unique_ptr<Widget> w(WidgetFactory());
    std::cout << "Initial w: " << w->size() << " ";
    for (auto x : *w) std::cout << x << " ";
    std::cout << std::endl;
    Double(&*w);
    Double(w.get());
}
