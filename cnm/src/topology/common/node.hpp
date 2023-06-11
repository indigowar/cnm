#ifndef HPP_CNM_TOPOLOGY_COMMON_NODE_HPP
#define HPP_CNM_TOPOLOGY_COMMON_NODE_HPP

#include <memory>
#include <string>

#include "connection/server_ctx.hpp"
#include "machines/machine.hpp"

namespace Cnm::Topology::Common {

class Node {
 public:
  explicit Node(std::string_view address,
                std::unique_ptr<Machines::Machine> machine)
      : address_{address}, machine_{std::move(machine)} {
    Machines::HostInfo host_info{};
    host_info.address = address_;
    host_info.name = machine_->getName();
    machine_->setHost(host_info);
  }

  virtual ~Node() { machine_->terminate(); }

  [[nodiscard]] virtual std::string_view getName() const noexcept {
    return machine_->getName();
  }

  [[nodiscard]] virtual std::string_view getType() const noexcept {
    return machine_->getType();
  }

  [[nodiscard]] virtual std::string_view getAddress() const noexcept {
    return address_;
  }

  virtual void setAddress(std::string_view addr) {
    address_ = addr;

    Machines::HostInfo host_info{};
    host_info.address = addr;
    host_info.name = machine_->getName();
    machine_->setHost(host_info);
  }

  virtual void serve(std::unique_ptr<Connection::ServerCtx>&& ctx) {
    machine_->serve(std::move(ctx));
  }

  // TODO: Replace isBusy logic with machine_->getServingAmount()
  virtual size_t getServingAmount() const noexcept {
    return machine_->getServingAmount();
  }

  virtual void replace(std::unique_ptr<Machines::Machine> machine) {
    machine_->terminate();
    machine_ = std::move(machine);
  }

  virtual void abort() { machine_->terminate(); }

 private:
  std::string_view address_;
  std::unique_ptr<Machines::Machine> machine_;
};
}  // namespace Cnm::Topology::Common
#endif  // HPP_CNM_TOPOLOGY_COMMON_NODE_HPP
