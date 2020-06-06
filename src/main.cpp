#include <iostream>
#include <string>

#include <constexpr_enum_map.hpp>
#include <string_view.hpp>

enum Fruit : uint32_t {
  Apple,
  Cherry,
  Lychee,
  Mango,
  Prune,

  DoNotUseMeMustBeLastEnumVariant
};

struct FruitProperty {
  Fruit _enum;
  bool soft;
  int32_t average_weight;
  std::string_view name;
};

DEFINE_CONSTEXPR_ENUM_MAP_TYPE(
  FruitProperty,
  FruitProperty{Fruit::Apple, false, 120, "apple"},
  FruitProperty{Fruit::Cherry, true, 4, "cherry"},
  FruitProperty{Fruit::Lychee, true, 20, "lychee"},
  FruitProperty{Fruit::Mango, true, 200, "mango"},
  FruitProperty{Fruit::Prune, true, 55, "prune"}
);

static constexpr auto fruit_properties = FruitProperty_enum_map_t::value();

auto is_heavy_soft_fruit(const Fruit fruit) -> bool {
  const auto fruit_property = fruit_properties[fruit];
  return fruit_property.soft && fruit_property.average_weight > 50;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    return 1;
  }

  // Bounds check becuase of user provided index.
  std::cout << fruit_properties.at(std::stoi(argv[1])).name << '\n';
}
