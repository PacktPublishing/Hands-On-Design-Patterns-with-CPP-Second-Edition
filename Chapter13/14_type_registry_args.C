#include <vector>
#include <iostream>
#include <memory>

struct BuildingSpec {
};

class Building {
  static size_t building_type_count;
  using BuildingFactory = Building* (*)(const BuildingSpec&);
  static std::vector<BuildingFactory> building_registry;

  public:
  static size_t RegisterBuilding(BuildingFactory factory) {
    building_registry.push_back(factory);
    return building_type_count++;
  }

  static auto MakeBuilding(size_t building_type, const BuildingSpec& spec) {
    BuildingFactory factory = building_registry[building_type];
    return std::unique_ptr<Building>(factory(spec));
  }
};
std::vector<Building::BuildingFactory> Building::building_registry;
size_t Building::building_type_count = 0;

struct FarmSpec : public BuildingSpec {
  bool with_pasture {};
  int number_of_stalls {};
  FarmSpec() = default;
  FarmSpec& SetPasture(bool with_pasture) { this->with_pasture = with_pasture; return *this; }
  FarmSpec& SetStalls(int number_of_stalls) { this->number_of_stalls = number_of_stalls; return *this; }
};

class Farm : public Building {
  public:
  explicit Farm(const FarmSpec& spec) { std::cout << "new Farm, " << (spec.with_pasture ? "with pasture" : "no pasture") << ", " << spec.number_of_stalls << " stalls" << std::endl; }
  static const size_t type_tag;
};
const size_t Farm::type_tag = RegisterBuilding([](const BuildingSpec& spec)->Building* { return new Farm(static_cast<const FarmSpec&>(spec)); });

struct ForgeSpec : public BuildingSpec {
  bool magic_forge {};
  int number_of_apprentices {};
  ForgeSpec() = default;
  ForgeSpec& SetMagic(bool magic_forge) { this->magic_forge = magic_forge; return *this; }
  ForgeSpec& SetApprentices(int number_of_apprentices) { this->number_of_apprentices = number_of_apprentices; return *this; }
};

class Forge : public Building {
  public:
  explicit Forge(const ForgeSpec& spec) { std::cout << "new Forge, " << spec.number_of_apprentices << " apprentices, " << (spec.magic_forge ? " magic" : "no magic") << std::endl; }
  static Building* MakeBuilding(const BuildingSpec& spec) { return new Forge(static_cast<const ForgeSpec&>(spec)); }
  static const size_t type_tag;
};
const size_t Forge::type_tag = RegisterBuilding(Forge::MakeBuilding);

int main() {
  std::unique_ptr<Building> farm = Building::MakeBuilding(Farm::type_tag, FarmSpec().SetPasture(true).SetStalls(2));
  std::unique_ptr<Building> forge = Building::MakeBuilding(Forge::type_tag, ForgeSpec().SetMagic(false).SetApprentices(4));
}

