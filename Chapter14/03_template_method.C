#include <iostream>
#include <memory>
#include <vector>
#include <string_view>

class Character {
    public:
    void CombatTurn() {
        if (MustFlee()) {
            Flee();
            return;
        }
        if (!Ready()) {
            GetReady();
            return; // Getting ready takes a full turn
        }
        CombatAction();
    }
    virtual bool MustFlee() const = 0;
    virtual bool Ready() const = 0;
    virtual void GetReady() = 0;
    virtual void CombatAction() = 0;
    void set_health(int h) { health_ = h; } // For testing
    protected:
    const std::string name_;
    Character(std::string_view name, int h) : name_(name), health_(h) {}
    int health_;
    void Flee() { std::cout << name_ << " runs for his life" << std::endl; }
    void Attack() { std::cout << name_ << " charges at his enemies" << std::endl; }
};


class Swordsman : public Character {
    bool wielded_sword_ {};
    void Wield() { wielded_sword_ = true; std::cout << name_ << " readies his sword" << std::endl; }
    public:
    explicit Swordsman(std::string_view name) : Character(name, 10) {}
    bool MustFlee() const override { return health_ < 5; }
    bool Ready() const override { return wielded_sword_; }
    void GetReady()override { Wield(); }
    void CombatAction()override { Attack(); }
};

class Wizard : public Character {
    int mana_ {2};
    bool scroll_ready_ {};
    void ReadScroll() { scroll_ready_ = true; std::cout << name_ << " unfurls the scroll of doom" << std::endl; }
    void CastSpell() { std::cout << name_ << " unleashes the wrath of Arcana" << std::endl; --mana_; }
    public:
    explicit Wizard(std::string_view name) : Character(name, 4) {}
    bool MustFlee() const override { return health_ < 2 || mana_ == 0; }
    bool Ready() const override { return scroll_ready_; }
    void GetReady() override { ReadScroll(); }
    void CombatAction() override { CastSpell(); }
};

int main() {
    Swordsman Arthur("Arthur");
    Wizard Merlin("Merlin");

    Arthur.CombatTurn();
    Merlin.CombatTurn();

    Arthur.set_health(3);
    Arthur.CombatTurn();
    Merlin.CombatTurn();
    Merlin.CombatTurn();
    Merlin.CombatTurn();
}
