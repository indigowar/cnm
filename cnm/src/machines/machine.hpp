#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP

#include <string_view>

#include "connection/server_ctx.hpp"

namespace Cnm::Machines {

struct HostInfo final {
  std::string_view address;
  std::string_view name;
  std::string_view topology;
  size_t current_speed;
};

class Machine {
 public:
  explicit Machine(HostInfo host_info) : info_{host_info} {}

  virtual size_t getServingAmount() const noexcept = 0;

  virtual std::string_view getName() const noexcept { return info_.name; }
  std::string_view getAddress() const noexcept { return info_.name; }

  virtual std::string_view getType() const noexcept = 0;

  virtual void serve(std::unique_ptr<Connection::ServerCtx>&& ctx) = 0;

  virtual void terminate() = 0;

  virtual void setHost(HostInfo info) final { info_ = info; }

 protected:
  HostInfo info_;
};
}  // namespace Cnm::Machines

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP
