#include <iostream>
#include <memory>

class Building {
    public:
    enum Type { FARM, FORGE, MILL, GUARDHOUSE, KEEP, CASTLE };
    virtual ~Building() {}
    static auto MakeBuilding(Type building_type);
};

class Farm : public Building {
    public:
    Farm() { std::cout << "new Farm" << std::endl; }
};

class Forge : public Building {
    public:
    Forge() { std::cout << "new Forge" << std::endl; }
};

auto Building::MakeBuilding(Type building_type) {
    using result_t = std::unique_ptr<Building>;
    switch (building_type) {
        case FARM: return result_t{new Farm};
        case FORGE: return result_t{new Forge};
        default: abort();
    }
}

int main() {
    std::unique_ptr<Building> farm = Building::MakeBuilding(Building::FARM);
    std::unique_ptr<Building> forge = Building::MakeBuilding(Building::FORGE);
}

