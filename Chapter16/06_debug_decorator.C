// 05 with dynamic memory allocation
#include <iostream>
#include <memory>
#include <cassert>
using std::cout;
using std::endl;

// Basic decorator
class Unit {
    public:
    virtual bool hit(Unit& target) { return attack() > target.defense(); }
    virtual double attack() = 0;
    virtual double defense() = 0;
    virtual ~Unit() = default;
    using ptr = std::unique_ptr<Unit>;
    protected:
    Unit(double strength, double armor) : strength_(strength), armor_(armor) {}
    double strength_;
    double armor_;
};

class Knight : public Unit {
    using Unit::Unit;
    public:
    double attack() override { double res = strength_ + sword_bonus_ + charge_bonus_; charge_bonus_ = 0; return res; }
    double defense() override { return armor_ + plate_bonus_; }
    void charge() { charge_bonus_ = 1; }
    using ptr = std::unique_ptr<Knight>;
    static ptr construct(double strength, double armor) { return ptr{new Knight(strength, armor)}; }
    protected:
    double charge_bonus_ {};
    static constexpr double sword_bonus_ = 2;
    static constexpr double plate_bonus_ = 3;
};

class Ogre : public Unit {
    using Unit::Unit;
    public:
    double attack() override { return strength_ + club_penalty_; }
    double defense() override { return armor_ + leather_penalty_; }
    using ptr = std::unique_ptr<Ogre>;
    static ptr construct(double strength, double armor) { return ptr{new Ogre(strength, armor)}; }
    protected:
    static constexpr double club_penalty_ = -1;
    static constexpr double leather_penalty_ = -1;
};

class Troll : public Unit {
    using Unit::Unit;
    public:
    double attack() override { return strength_ + mace_bonus_; }
    double defense() override { return armor_ + hide_bonus_; }
    using ptr = std::unique_ptr<Troll>;
    static ptr construct(double strength, double armor) { return ptr{new Troll(strength, armor)}; }
    protected:
    static constexpr double mace_bonus_ = 3;
    static constexpr double hide_bonus_ = 8;
};

template <typename To, typename From> static std::unique_ptr<To> move_cast(std::unique_ptr<From>& p) {
#ifndef NDEBUG
    auto p1 = std::unique_ptr<To>(dynamic_cast<To*>(p.release()));
    assert(p1);
    return p1;
#else 
    return std::unique_ptr<To>(static_cast<To*>(p.release()));
#endif
}

template <typename U> class VeteranUnit : public U {
    public:
    double attack() override { return U::attack() + strength_bonus_; }
    double defense() override { return U::defense() + armor_bonus_; }
    using ptr = std::unique_ptr<VeteranUnit>;
    template <typename P> static ptr construct(P&& p, double strength_bonus, double armor_bonus) { return ptr{new VeteranUnit(p, strength_bonus, armor_bonus)}; }
    protected:
    template <typename UU> VeteranUnit(std::unique_ptr<UU>& p, double strength_bonus, double armor_bonus) : U(std::move(*move_cast<U>(p))), strength_bonus_(strength_bonus), armor_bonus_(armor_bonus) {}
    private:
    double strength_bonus_;
    double armor_bonus_;
};

template <typename U> class DebugDecorator : public U {
    using U::U;
    public:
    template <typename P> DebugDecorator(std::unique_ptr<P>& p) : U(std::move(*move_cast<U>(p))) {}
    double attack() override { double res = U::attack(); cout << "Attack: " << res << endl; return res; }
    double defense() override { double res = U::defense(); cout << "Defense: " << res << endl; return res; }
    using ptr = std::unique_ptr<DebugDecorator>;
    template <typename... Args> static ptr construct(Args&&... args) { return ptr{new DebugDecorator(std::forward<Args>(args)...)}; }
};

int main() {
    Knight::ptr k(DebugDecorator<Knight>::construct(10, 5));
    Unit::ptr o(DebugDecorator<Ogre>::construct(12, 2));
    k->charge();
    cout << "Knight hits Ogre: " << k->hit(*o) << endl;
    Unit::ptr t(Troll::construct(14, 10));
    cout << "Knight hits Troll: " << k->hit(*t) << endl;
    Knight::ptr vk(DebugDecorator<VeteranUnit<Knight>>::construct(k, 7, 2));
    cout << "Veteran Knight hits Troll: " << vk->hit(*t) << endl;
    Unit::ptr dt(DebugDecorator<Troll>::construct(t));
    cout << "Veteran Knight hits Troll (debug): " << vk->hit(*dt) << endl;
    Unit::ptr vo(DebugDecorator<VeteranUnit<Ogre>>::construct(o, 1, 9));
    cout << "Veteran Knight hits Veteran Ogre: " << vk->hit(*vo) << endl;
    Unit::ptr vvo(DebugDecorator<VeteranUnit<VeteranUnit<Ogre>>>::construct(vo, 1, 9));
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk->hit(*vvo) << endl;
    vk->charge();
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk->hit(*vvo) << endl;
}
