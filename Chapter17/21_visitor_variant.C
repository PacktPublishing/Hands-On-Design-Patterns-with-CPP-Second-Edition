#include <iostream>
#include <memory>
#include <variant>
#include <string_view>

using Pet = std::variant<class Cat, class Dog, class Lorikeet>;

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

class Lorikeet {
    public:
    Lorikeet(std::string_view body, std::string_view head) : body_(body), head_(head) {}
    std::string color() const { return body_ + " and " + head_; }
    private:
    const std::string body_;
    const std::string head_;
};

class FeedingVisitor {
    public:
    void operator()(const Cat& c) { std::cout << "Feed tuna to the " << c.color() << " cat" << std::endl; }
    void operator()(const Dog& d) { std::cout << "Feed steak to the " << d.color() << " dog" << std::endl; }
    void operator()(const Lorikeet& l) { std::cout << "Feed grain to the " << l.color() << " bird" << std::endl; }
};

class PlayingVisitor {
    public:
    void operator()(const Cat& c) { std::cout << "Play with feather with the " << c.color() << " cat" << std::endl; }
    void operator()(const Dog& d) { std::cout << "Play fetch with the " << d.color() << " dog" << std::endl; }
    void operator()(const Lorikeet& l) { std::cout << "Teach words to the " << l.color() << " bird" << std::endl; }
};

void walk(const Pet& p) {
    class WalkingVisitor {
        public:
        void operator()(const Cat& c) { std::cout << "Let the " << c.color() << " cat out" << std::endl; }
        void operator()(const Dog& d) { std::cout << "Take the " << d.color() << " dog for a walk" << std::endl; }
        void operator()(const Lorikeet& l) { std::cout << "Release the " << l.color() << " bird to fly" << std::endl; }
    } v;
    std::visit(v, p);
}

int main() {
    Pet c = Cat("orange");
    Pet d = Dog("brown");
    Pet l = Lorikeet("yellow", "green");

    FeedingVisitor fv;
    std::visit(fv, c);
    std::visit(fv, d);
    std::visit(fv, l);

    PlayingVisitor pv;
    std::visit(pv, c);
    std::visit(pv, d);
    std::visit(pv, l);

    walk(c);
    walk(d);
    walk(l);
}
