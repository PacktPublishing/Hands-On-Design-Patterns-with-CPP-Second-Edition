#include <vector>
#include <iostream>
#include <memory>
#include <functional>

class Building {
    static size_t building_type_count;
    using BuildingFactory = std::function<Building*()>;
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
};
std::vector<Building::BuildingFactory> Building::building_registry;
size_t Building::building_type_count = 0;

class Farm : public Building {
    public:
    Farm() { std::cout << "new Farm" << std::endl; }
    static const size_t type_tag;
};
const size_t Farm::type_tag = RegisterBuilding([]()->Building* { return new Farm; });

class Forge : public Building {
    public:
    Forge() { std::cout << "new Forge" << std::endl; }
    static const size_t type_tag;
};
class ForgeFactory {
    public:
    Building* operator()() const { return new Forge; }
};
const size_t Forge::type_tag = RegisterBuilding(ForgeFactory{});

int main() {
    std::unique_ptr<Building> b0 = Building::MakeBuilding(Farm::type_tag);
    std::unique_ptr<Building> b1 = Building::MakeBuilding(Forge::type_tag);
}

