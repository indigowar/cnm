#ifndef HPP_CNM_CORE_MACHINE_HOST_INFO_HPP
#define HPP_CNM_CORE_MACHINE_HOST_INFO_HPP

#include <cstdint>
#include <string>

namespace Cnm {

class HostInfo final {
 public:
  HostInfo() = default;

  HostInfo(std::string_view name, std::string_view address)
      : name{name}, address{address} {}

  [[nodiscard]] const std::string& getName() const noexcept { return name; }
  [[nodiscard]] const std::string& getAddress() const noexcept {
    return address;
  }

  bool operator==(const HostInfo& other) const {
    return name == other.name && address == other.address;
  }

  bool operator!=(const HostInfo& other) const { return !(*this == other); }

  static HostInfo generate(std::string name, uint8_t, uint8_t, uint8_t,
                           uint8_t);

  static HostInfo generate(std::string name);

 private:
  std::string name;
  std::string address;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_HOST_INFO_HPP
