#include <iostream>
#include <memory>
#include <variant>
#include <string_view>

using Pet = std::variant<class Cat, class Dog>;

class PetBase {
    public:
    PetBase(std::string_view color) : color_(color) {}
    const std::string& color() const { return color_; }
    private:
    const std::string color_;
};

class Cat : private PetBase {
    public:
    using PetBase::PetBase;
    using PetBase::color;
};

class Dog : private PetBase {
    public:
    using PetBase::PetBase;
    using PetBase::color;
};

class CareVisitor {
    public:
    void operator()(const Cat& c1, const Cat& c2) { std::cout << "Let the " << c1.color() << " and the " << c2.color() << " cats play" << std::endl; }
    void operator()(const Dog& d, const Cat& c) { std::cout << "Keep the " << d.color() << " dog safe from the vicious " << c.color() << " cat" << std::endl; }
    void operator()(const Cat& c, const Dog& d) { (*this)(d, c); }
    void operator()(const Dog& d1, const Dog& d2) { std::cout << "Take the " << d1.color() << " and the " << d2.color() << " dogs for a walk" << std::endl; }
};

int main() {
    Pet c1 = Cat("orange");
    Pet c2 = Cat("black");
    Pet d = Dog("brown");

    CareVisitor cv;
    std::visit(cv, c1, c2);
    std::visit(cv, c1, d);

}
