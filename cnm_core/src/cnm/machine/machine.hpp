#ifndef HPP_CNM_CORE_MACHINE_MACHINE_HPP
#define HPP_CNM_CORE_MACHINE_MACHINE_HPP

#include <memory>
#include <mutex>
#include <string_view>

#include "cnm/core/message.hpp"
#include "cnm/core/object.hpp"
#include "cnm/machine/communicator.hpp"
#include "cnm/machine/host_info.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

class Machine : public Object {
 public:
  Machine(std::string_view type, size_t serving_limit, HostInfo host_info,
          std::shared_ptr<Communicator> communicator)
      : type{type},
        serving_limit{serving_limit},
        host_info{host_info},
        communicator{std::move(communicator)} {}

  ~Machine() override { communicator->disconnect(host_info); }

  [[nodiscard]] std::string_view getType() const noexcept { return type; }

  [[nodiscard]] size_t getServingLimit() const noexcept {
    return serving_limit;
  }

  [[nodiscard]] HostInfo getHostInfo() const noexcept {
    auto lock = makeLock();
    return host_info;
  }

  void setHostInfo(HostInfo new_host_info) {
    auto lock = makeLock();
    host_info = new_host_info;
  }

  void setCommunicator(std::shared_ptr<Communicator> new_communicator) {
    auto lock = makeLock();
    communicator = std::move(new_communicator);
  }

  [[nodiscard]] virtual size_t getCurrentServingAmount() const noexcept = 0;

  [[nodiscard]] std::unique_lock<std::mutex> makeLock() const noexcept {
    return std::unique_lock(mutex);
  }

  [[nodiscard]] virtual result_t<MessageBatch> serve(MessageBatch) = 0;

 protected:
  HostInfo host_info;
  std::shared_ptr<Communicator> communicator;

 private:
  const std::string_view type;
  const size_t serving_limit;

  mutable std::mutex mutex;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_MACHINE_HPP
