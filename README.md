# constexpr enum map

Associate arbitrary constexpr values to enum values.

```cpp
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
```

## Design Goals

- Easy to use correctly -> produces `std::array`, intuitive semantics
- Hard to use incorrectly -> compile time property checking
- Excellent code gen -> https://godbolt.org/z/WtV2aw
- Little compile time impact -> no TMP recursion
- Minimal static init work -> constexpr value in static section

For example compare that to `std::unordered_map` https://godbolt.org/z/JTGaxo:
- ~10x more binary bloat
- Much worse code gen
- No guarantee that all enum variants have a mapping
- Much more static init work, including heap allocations

## When does it not make sense to use this

- If the associated value is only a simple integral -> Use enum values
- If the mapping needs to be modified at runtime -> Use an unordered_map

## Compile time impact

TODO

## Options

- Some C++14 std library functionality is backported, if you want to use the std versions of them configure your compiler to declare itself at least C++14 compatible and define CEM_NO_STD_SHIM, clang example `clang -std=c++14 -DCEM_NO_STD_SHIM`.


## Getting Started

- Include the `constexpr_enum_map.hpp` header

### Prerequisites

- C++11 compiler
- `constexpr_enum_map.hpp` found in include paths

### Installing

- Download the file eg. `wget https://raw.githubusercontent.com/Voultapher/constexpr-enum-map/master/constexpr_enum_map.hpp`

## Running the tests

TODO


## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md)
for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available,
see the [tags on this repository](https://github.com/Voultapher/constexpr-enum-map/tags).

## Authors

* **Lukas Bergdoll** - *Initial work* - [Voultapher](https://github.com/Voultapher)

See also the list of [contributors](https://github.com/Voultapher/constexpr-enum-map/contributors)
who participated in this project.

## License

This project is licensed under the Apache License, Version 2.0 -
see the [LICENSE.md](LICENSE.md) file for details.
