#include <iostream>
#include <memory>

struct BuildingSpec;

class Building {
    public:
    enum Type { FARM, FORGE, MILL, GUARDHOUSE, KEEP, CASTLE };
    virtual ~Building() {}
    static auto MakeBuilding(const BuildingSpec& building_spec);
};

struct BuildingSpec {
  virtual Building::Type building_type() const = 0;
};

struct FarmSpec : public BuildingSpec {
  Building::Type building_type() const override {
    return Building::FARM;
  }
  bool with_pasture {};
  int number_of_stalls {};
  FarmSpec() = default;
  FarmSpec& SetPasture(bool with_pasture) { this->with_pasture = with_pasture; return *this; }
  FarmSpec& SetStalls(int number_of_stalls) { this->number_of_stalls = number_of_stalls; return *this; }
};

struct ForgeSpec : public BuildingSpec {
  Building::Type building_type() const override {
    return Building::FORGE;
  }
  bool magic_forge {};
  int number_of_apprentices {};
  ForgeSpec() = default;
  ForgeSpec& SetMagic(bool magic_forge) { this->magic_forge = magic_forge; return *this; }
  ForgeSpec& SetApprentices(int number_of_apprentices) { this->number_of_apprentices = number_of_apprentices; return *this; }
};

class Farm : public Building {
    public:
    explicit Farm(const FarmSpec& spec) { std::cout << "new Farm, " << (spec.with_pasture ? "with pasture" : "no pasture") << ", " << spec.number_of_stalls << " stalls" << std::endl; }
};

class Forge : public Building {
    public:
    explicit Forge(const ForgeSpec& spec) { std::cout << "new Forge, " << spec.number_of_apprentices << " apprentices, " << (spec.magic_forge ? " magic" : "no magic") << std::endl; }
};

auto Building::MakeBuilding(const BuildingSpec& building_spec) {
    using result_t = std::unique_ptr<Building>;
    switch (building_spec.building_type()) {
        case FARM: return result_t{new Farm(static_cast<const FarmSpec&>(building_spec))};
        case FORGE: return result_t{new Forge(static_cast<const ForgeSpec&>(building_spec))};
        default: abort();
    }
}

int main() {
    std::unique_ptr<Building> farm = Building::MakeBuilding(FarmSpec().SetPasture(true).SetStalls(2));
    std::unique_ptr<Building> forge = Building::MakeBuilding(ForgeSpec().SetMagic(false).SetApprentices(4));
}

