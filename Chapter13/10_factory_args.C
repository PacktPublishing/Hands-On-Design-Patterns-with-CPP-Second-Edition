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
    explicit Farm(double size) { std::cout << "new Farm, " << size << " acres" << std::endl; }
};

class Forge : public Building {
    public:
    static constexpr size_t weaponsmith = 0x1;
    static constexpr size_t welder = 0x2;
    static constexpr size_t farrier = 0x4;
    Forge(size_t staff, size_t services) {
        std::cout << "new Forge, " << staff << " craftsmen:";
        if (services & weaponsmith) std::cout << " weaponsmith";
        if (services & welder) std::cout << " welder";
        if (services & farrier) std::cout << " farrier";
        std::cout << std::endl;
    }
};

template <typename T, typename... Args> auto new_T(Args&&... args) -> decltype(T(std::forward<Args>(args)...))* {
    return new T(std::forward<Args>(args)...);
}
template <typename T> T* new_T(...) { abort(); return nullptr; }

template <typename... Args> 
auto Building::MakeBuilding(Type building_type, Args&&... args) {
    using result_t = std::unique_ptr<Building>;
    switch (building_type) {
        case FARM: return result_t{new_T<Farm>(std::forward<Args>(args)...)};
        case FORGE: return result_t{new_T<Forge>(std::forward<Args>(args)...)};
        default: abort();
    }
}

int main() {
    std::unique_ptr<Building> farm = Building::MakeBuilding(Building::FARM, 42);
    std::unique_ptr<Building> forge = Building::MakeBuilding(Building::FORGE, 2, Forge::weaponsmith | Forge::welder | Forge::farrier);
}

