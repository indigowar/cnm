#ifndef HPP_CNM_CORE_MACHINE_SERVER_HPP
#define HPP_CNM_CORE_MACHINE_SERVER_HPP

#include <functional>

#include "cnm/core/message.hpp"
#include "cnm/machine/host_info.hpp"
#include "cnm/machine/interactor.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/utils/thread_pool.hpp"

namespace Cnm {

class Server : public Machine {
 public:
  using Logic = std::function<result_t<MessageBatch>(
      const HostInfo&, Interactor*, MessageBatch)>;

  Server(Logic&&, size_t concurrency_limit, HostInfo);

  void start() override;

  void stop() override;

  void invoke() override;

  void froze() override;

  result_t<MessageBatch> serve(MessageBatch) override;

 private:
  Utils::ThreadPool pool;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_SERVER_HPP
