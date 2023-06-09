#ifndef HPP_CNM_LIB_TOPOLOGY_RING_HPP
#define HPP_CNM_LIB_TOPOLOGY_RING_HPP

#include "machines/machine.hpp"
#include "topology/ring/iterator.hpp"
#include "topology/ring/node.hpp"

namespace Cnm::Topology::Ring {

class Topology final {
 public:
  explicit Topology() = default;

  static std::string_view getType() noexcept { return "Ring"; }

  [[nodiscard]] size_t getNetSpeed() const noexcept;
  void setNetSpeed(size_t);

  std::string_view addMachine(std::unique_ptr<Machines::Machine>);
  std::string_view addMachine(std::unique_ptr<Machines::Machine>,
                              std::string_view);

  void removeMachine(std::string_view);

  void stop();

  Iterator begin();
  Iterator end();

  ReverseIterator rbegin();
  ReverseIterator rend();

 private:
  std::string_view generateFreeAddress() const noexcept;

  std::shared_ptr<Node> head_;
  size_t net_speed_;
};

}  // namespace Cnm::Topology::Ring

#endif  // HPP_CNM_LIB_TOPOLOGY_RING_HPP