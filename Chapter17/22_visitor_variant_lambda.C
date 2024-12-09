#include <iostream>
#include <memory>
#include <variant>
#include <string_view>

template <typename... T> struct overloaded : T... { using T::operator()...; };
template <typename... T> overloaded( T...)->overloaded<T...>;

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

#define SAME(v, T) std::is_same_v<std::decay_t<decltype(v)>, T>
auto fv = [](const auto& p) {
    if constexpr (SAME(p, Cat)) { std::cout << "Feed tuna to the " << p.color() << " cat" << std::endl; }
    else if constexpr (SAME(p, Dog)) { std::cout << "Feed steak to the " << p.color() << " dog" << std::endl; }
    else if constexpr (SAME(p, Lorikeet)) { std::cout << "Feed grain to the " << p.color() << " bird" << std::endl; }
    else abort();
};
#undef SAME

auto pv = overloaded {
    [](const Cat& c) { std::cout << "Play with feather with the " << c.color() << " cat" << std::endl; },
    [](const Dog& d) { std::cout << "Play fetch with the " << d.color() << " dog" << std::endl; },
    [](const Lorikeet& l) { std::cout << "Teach words to the " << l.color() << " bird" << std::endl; }
};

void walk(const Pet& p) {
    auto v = overloaded {
        [](const Cat& c) { std::cout << "Let the " << c.color() << " cat out" << std::endl; },
        [](const Dog& d) { std::cout << "Take the " << d.color() << " dog for a walk" << std::endl; },
        [](const Lorikeet& l) { std::cout << "Release the " << l.color() << " bird to fly" << std::endl; }
    };
    std::visit(v, p);
}

int main() {
    Pet c = Cat("orange");
    Pet d = Dog("brown");
    Pet l = Lorikeet("yellow", "green");

    std::visit(fv, c);
    std::visit(fv, d);
    std::visit(fv, l);

    std::visit(pv, c);
    std::visit(pv, d);
    std::visit(pv, l);

    walk(c);
    walk(d);
    walk(l);
}
