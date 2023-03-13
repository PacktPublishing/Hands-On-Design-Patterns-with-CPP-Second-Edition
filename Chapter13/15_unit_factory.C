#include <vector>
#include <iostream>
#include <memory>

class Unit {};
class Knight : public Unit {
    public:
    Knight() { std::cout << "new Knight" << std::endl; }
};
class Mage : public Unit {
    public:
    Mage() { std::cout << "new Mage" << std::endl; }
};
class Spider : public Unit {
    public:
    Spider() { std::cout << "new Spider" << std::endl; }
};

class Building {
    static size_t building_type_count;
    using BuildingFactory = Building* (*)();
    static std::vector<BuildingFactory> building_registry;

    public:
    static size_t RegisterBuilding(BuildingFactory factory) {
        building_registry.push_back(factory);
        return building_type_count++;
    }

    static auto MakeBuilding(size_t building_type) {
        BuildingFactory factory = building_registry[building_type];
        return std::unique_ptr<Building>(factory());
    }

    virtual ~Building() {}
    virtual Unit* MakeUnit() const = 0;
};
std::vector<Building::BuildingFactory> Building::building_registry;
size_t Building::building_type_count = 0;

class Castle : public Building {
    public:
    Castle() { std::cout << "new Castle" << std::endl; }
    static const size_t type_tag;
    Knight* MakeUnit() const override { return new Knight; }
};
const size_t Castle::type_tag = RegisterBuilding([]()->Building* { return new Castle; });

class Tower : public Building {
    public:
    Tower() { std::cout << "new Tower" << std::endl; }
    static const size_t type_tag;
    Mage* MakeUnit() const override { return new Mage; }
};
const size_t Tower::type_tag = RegisterBuilding([]()->Building* { return new Tower; });

class Mound : public Building {
    public:
    Mound() { std::cout << "new Mound" << std::endl; }
    static const size_t type_tag;
    Spider* MakeUnit() const override { return new Spider; }
};
const size_t Mound::type_tag = RegisterBuilding([]()->Building* { return new Mound; });

int main() {
    std::vector<std::unique_ptr<Building>> buildings;
    buildings.push_back(Building::MakeBuilding(Castle::type_tag));
    buildings.push_back(Building::MakeBuilding(Tower::type_tag));
    buildings.push_back(Building::MakeBuilding(Mound::type_tag));
    std::vector<std::unique_ptr<Unit>> units;
    for (const auto& b : buildings) {
        units.emplace_back(b->MakeUnit());
    }
}

