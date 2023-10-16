#ifndef HPP_CNM_CORE_TOPOLOGY_BASE_TOPOLOGY_HPP
#define HPP_CNM_CORE_TOPOLOGY_BASE_TOPOLOGY_HPP

#include <memory>

#include "cnm/core/object.hpp"
#include "cnm/machine/host_info.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/topology/base/node_iterator.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

// Topology - an interface to all existing topologies.
class Topology : public Object {
 public:
  // addMachine - adds new machine into topology,
  // returns the HostInfo of the node or the error.
  virtual result_t<HostInfo> addMachine(std::unique_ptr<Machine>&&) = 0;

  virtual result_t<HostInfo> addMachine(std::unique_ptr<Machine>&&,
                                        HostInfo) = 0;

  // addMachineWithName - adds new machine into topology, but with specified
  // name, returns HostInfo of the node or the error.
  virtual result_t<HostInfo> addMachineWithName(std::unique_ptr<Machine>&&,
                                                std::string_view name) = 0;

  // addMachineWithAddress - adds new machine into topology, but with specified
  // address, returns HostInfo of the node or the error.
  virtual result_t<HostInfo> addMachineWithAddress(
      std::unique_ptr<Machine>&&, std::string_view address) = 0;

  // deleteMachine - deletes a machine with given HostInfo.
  virtual result_t<bool> deleteMachine(HostInfo) = 0;

  // validate - checks if the node are connected right.
  [[nodiscard]] virtual result_t<bool> validate() const noexcept = 0;

  [[nodiscard]] virtual std::string_view getType() const noexcept = 0;

  [[nodiscard]] virtual size_t getNetworkSpeed() const noexcept = 0;
  virtual void setNetworkSpeed(size_t speed) = 0;

  // begin - returns an iterator for the first element.
  template <typename T, NodeIterator I>
    requires std::derived_from<T, Topology> || requires(T t) {
      { t.begin() } -> std::same_as<I>;
      { t.end() } -> std::same_as<I>;
    }
  static auto begin(T& t) {
    return t.begin();
  }

  // end - returns an iterator on the place after the last element.
  template <typename T, NodeIterator I>
    requires std::derived_from<T, Topology> || requires(T t) {
      { t.begin() } -> std::same_as<I>;
      { t.end() } -> std::same_as<I>;
    }
  static auto end(T& t) {
    return t.end();
  }
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_TOPOLOGY_BASE_TOPOLOGY_HPP
