#include <iostream>
#include <vector>
#include <memory>

struct Widget : public std::vector<int> {
    Widget(std::initializer_list<int> l) : std::vector<int>(l) {
        std::cout << "New Widget constructed" << std::endl;
    }
    virtual void print() const { for (int i : *this) std::cout << i << " "; std::cout << std::endl; }
    virtual ~Widget() { std::cout << "Deleting Widget" << std::endl; }
};

struct FancyWidget : public Widget {
    using Widget::Widget;
    void print() const override { // Fancy print
        std::cout << "==[";
        for (size_t i = 0; i != this->size(); ++i) {
            if (i == 0) std::cout << (*this)[i];
            else std::cout << ", " << (*this)[i];
        }
        std::cout << "]==" << std::endl;
    }
};

class WidgetProcessor {
  public:
  WidgetProcessor(std::initializer_list<Widget*> w) : w_(w) {}
  void print() const { for (const Widget* w : w_) w->print(); }
  ~WidgetProcessor() { /* DO NOT delete widgets in w_! */ }
  private:
  std::vector<Widget*> w_;	// I do not own w_
};

int main() {
    std::unique_ptr<Widget> w{new Widget{{1, 2, 3}}};
    std::unique_ptr<Widget> fw{new FancyWidget{{4, 5, 6}}};
    WidgetProcessor wp{{w.get(), fw.get()}};
    wp.print();
    std::cout << "widgets deleted by the owner" << std::endl;
}

