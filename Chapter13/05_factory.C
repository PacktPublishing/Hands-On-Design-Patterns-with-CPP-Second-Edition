#include <iostream>

enum Buildings {
    FARM, FORGE, MILL, GUARDHOUSE, KEEP, CASTLE
};

class Building {
    public:
    virtual ~Building() {}
};


class Farm : public Building {
    public:
    Farm() { std::cout << "new Farm" << std::endl; }
};

class Forge : public Building {
    public:
    Forge() { std::cout << "new Forge" << std::endl; }
};

Building* MakeBuilding(Buildings building_type) {
  switch (building_type) {
    case FARM: return new Farm;
    case FORGE: return new Forge;
    default: abort();
  }
}

int main() {
    Building* farm = MakeBuilding(FARM);
    Building* forge = MakeBuilding(FORGE);
    delete farm;
    delete forge;
}

