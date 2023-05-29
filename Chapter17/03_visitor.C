// 02 with polymorphic calls for visitor too
#include <iostream>
#include <string>
#include <string_view>
#include <memory>

class Cat;
class Dog;

class PetVisitor {
    public:
    virtual ~PetVisitor() {}
    virtual void visit(Cat* c) = 0;
    virtual void visit(Dog* d) = 0;
};

class Pet {
    public:
    virtual ~Pet() {}
    Pet(std::string_view color) : color_(color) {}
    const std::string& color() const { return color_; }
    virtual void accept(PetVisitor& v) = 0;
    private:
    std::string color_;
};

class Cat : public Pet {
    public:
    Cat(std::string_view color) : Pet(color) {}
    void accept(PetVisitor& v) override { v.visit(this); }
};

class Dog : public Pet {
    public:
    Dog(std::string_view color) : Pet(color) {}
    void accept(PetVisitor& v) override { v.visit(this); }
};

class FeedingVisitor : public PetVisitor {
    public:
    void visit(Cat* c) override { std::cout << "Feed tuna to the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Feed steak to the " << d->color() << " dog" << std::endl; }
};

class PlayingVisitor : public PetVisitor {
    public:
    void visit(Cat* c) override { std::cout << "Play with feather with the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Play fetch with the " << d->color() << " dog" << std::endl; }
};

void dispatch(Pet& p, PetVisitor& v) { p.accept(v); }

int main() {
    std::unique_ptr<Pet> c(new Cat("orange"));
    std::unique_ptr<Pet> d(new Dog("brown"));

    std::unique_ptr<PetVisitor> fv(new FeedingVisitor);
    c->accept(*fv);
    d->accept(*fv);

    std::unique_ptr<PetVisitor> pv(new PlayingVisitor);
    c->accept(*pv);
    d->accept(*pv);

    // Showcase double dispatch.
    dispatch(*c, *pv);
    dispatch(*d, *fv);
}
