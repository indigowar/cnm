#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP

#include <string_view>

#include "connection/server_ctx.hpp"

namespace Cnm::Machines {

struct HostInfo final {
  std::string_view address;
  std::string_view name;
};

class Machine {
 public:
  // The Interactor probably should be moved from Machines to the topologies,
  // and be renamed to DNS or something like that.
  //
  // But due to the lack of time to implement it properly, it's an interface
  // here.
  //
  // If this project will be continued( which  I doubt), then:
  // TODO: Move from Machine to topology/common and rename to DNS.
  struct Interactor {
    virtual std::vector<std::tuple<std::string_view, std::string_view>>
    getMachinesInNetwork() = 0;

    virtual std::vector<std::tuple<std::string_view, std::string_view>>
    getServersInNetwork() = 0;

    virtual std::vector<std::tuple<std::string_view, std::string_view>>
    getOfficeEquipmentInNetwork() = 0;

    bool isConnected();
    std::unique_ptr<Connection::RequesterCtx> makeConnection();
    void disconnect();
  };

  explicit Machine(HostInfo host_info) : info_{host_info} {}

  virtual size_t getServingAmount() const noexcept = 0;

  std::string_view getName() const noexcept { return info_.name; }
  std::string_view getAddress() const noexcept { return info_.name; }

  virtual std::string_view getType() const noexcept = 0;

  virtual void serve(std::unique_ptr<Connection::ServerCtx>&& ctx) = 0;

  virtual void terminate() = 0;

  void setHost(HostInfo info) { info_ = info; }

  void set_interactor(Interactor* interactor) { interactor_ = interactor; }

  virtual void start() = 0;
  virtual void stop() = 0;

 protected:
  HostInfo info_;
  Interactor* interactor_;
};
}  // namespace Cnm::Machines

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP
