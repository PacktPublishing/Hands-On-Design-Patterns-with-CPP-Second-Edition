#include <iostream>
#include <memory>

class Building {
    public:
    enum Type { FARM, FORGE, MILL, GUARDHOUSE, KEEP, CASTLE };
    virtual ~Building() {}
    template <typename... Args> static auto MakeBuilding(Type building_type, Args&&... args);
};

class Farm : public Building {
    public:
    explicit Farm(...) { abort(); }
    explicit Farm(double size) { std::cout << "new Farm, " << size << " acres" << std::endl; }
};

class Forge : public Building {
    public:
    static constexpr size_t weaponsmith = 0x1;
    static constexpr size_t welder = 0x2;
    static constexpr size_t farrier = 0x4;
    explicit Forge(...) { abort(); }
    Forge(size_t staff, size_t services) {
        std::cout << "new Forge, " << staff << " craftsmen:";
        if (services & weaponsmith) std::cout << " weaponsmith";
        if (services & welder) std::cout << " welder";
        if (services & farrier) std::cout << " farrier";
        std::cout << std::endl;
    }
};

template <typename... Args> 
auto Building::MakeBuilding(Type building_type, Args&&... args) {
    using result_t = std::unique_ptr<Building>;
    switch (building_type) {
        case FARM: return result_t{new Farm(std::forward<Args>(args)...)};
        case FORGE: return result_t{new Forge(std::forward<Args>(args)...)};
        default: abort();
    }
}

int main() {
    std::unique_ptr<Building> farm = Building::MakeBuilding(Building::FARM, 42);
    std::unique_ptr<Building> forge = Building::MakeBuilding(Building::FORGE, 2, Forge::weaponsmith | Forge::welder | Forge::farrier);
}

