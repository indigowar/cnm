#ifndef HPP_CNM_CORE_MACHINE_HOST_INFO_HPP
#define HPP_CNM_CORE_MACHINE_HOST_INFO_HPP

#include <string_view>

namespace Cnm {

struct HostInfo final {
  std::string_view name;
  std::string_view address;

  bool operator==(const HostInfo& other) const {
    return name == other.name && address == other.address;
  }

  bool operator!=(const HostInfo& other) const { return !(*this == other); }
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_HOST_INFO_HPP
