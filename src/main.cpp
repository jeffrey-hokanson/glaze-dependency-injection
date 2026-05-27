#include <format>
#include <iostream>
#include <string>
#include <string>
#include <string_view>
#include <variant>

#include <glaze/glaze.hpp>

class Vehicle {
  public:
    virtual std::string description() = 0;
    virtual ~Vehicle() = default;
};

class Car : public Vehicle{
  public:
    struct Config {
      struct glaze {static constexpr std::string_view name = "Car";};
      using Impl = Car;
      double horsepower = 500;
      int seats = 4;
    };

    explicit Car(Config config): _config(config) {}

    std::string description() override {
      return std::format("Car seats {}, horsepower {}", _config.seats, _config.horsepower);
    }

  private:
    Config _config;
};

class Truck : public Vehicle {
  public: 
    struct Config {
      struct glaze {static constexpr std::string_view name = "Truck";};
      using Impl = Truck;
      double horsepower = 1000;
      double bed_length_ft = 5;
    };

    explicit Truck(Config config): _config(config) {}
    
    std::string description() override {
      return std::format("Truck horsepower {}, bed length {}", _config.horsepower, _config.bed_length_ft);

    }
  private:
    Config _config;
};

// This is the magic factory 
template <typename Base, typename ConfigVariant>
std::unique_ptr<Base> make(const ConfigVariant& config) {
  return std::visit(
      [](const auto& c) -> std::unique_ptr<Base> {
        using Config = std::decay_t<decltype(c)>;
        static_assert(requires {typename Config::Impl;}, "Config must have using Impl=...");
        using Impl = typename Config::Impl;
        static_assert(std::is_constructible_v<Impl, const Config&>,
            "Impl must be constructable from const Config&");
        return std::make_unique<Impl>(c);
      },
      config
    );
}



using VehicleTypeConfig = std::variant<Car::Config, Truck::Config>;

struct VehicleConfig {
  VehicleTypeConfig vehicle = Truck::Config{.bed_length_ft = 2}; // non-default config
  std::string owner = "me";
};

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <argument>\n";
    return 1;
  }
  
  std::string fname = argv[1];
  std::cout << std::format("Reading in {}\n", fname);

  std::ifstream file(fname);
  if (!file) {
      std::cerr << "Failed to open file\n";
      return 1;
  }

  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());


  VehicleConfig my_config;
  auto ec_read = glz::read_json(my_config, content);
  if (ec_read) {
    std::cerr << "error reading file\n";
    return 1;
  }

  auto json = glz::write_json(my_config).value_or("error");
  std::cout << "JSON config (including defaults if not specified)\n" << json << "\n";

  auto my_vehicle = make<Vehicle, VehicleTypeConfig>(my_config.vehicle); 
  std::cout << "Vehicle description: " << my_vehicle->description() << "\n";
  return 0;
}
