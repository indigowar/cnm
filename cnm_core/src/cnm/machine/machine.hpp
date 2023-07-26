#ifndef HPP_CNM_CORE_MACHINE_MACHINE_HPP
#define HPP_CNM_CORE_MACHINE_MACHINE_HPP

#include <memory>

#include "cnm/core/message.hpp"
#include "cnm/core/object.hpp"
#include "cnm/machine/host_info.hpp"
#include "cnm/machine/interactor.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

// Base class for all running machines in the network
class Machine : public Object {
 public:
  Machine(HostInfo host_info, std::shared_ptr<Interactor> interactor)
      : host{host_info}, interactor{std::move(interactor)} {}

  virtual ~Machine() { interactor->disconnect(host); }

  HostInfo getHostInfo() const noexcept { return host; }

  void setHostInfo(HostInfo info) { host = info; }

  virtual result_t<MessageBatch> serve(MessageBatch) = 0;

 private:
  HostInfo host;
  std::shared_ptr<Interactor> interactor;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_MACHINE_HPP
