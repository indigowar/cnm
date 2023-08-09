#ifndef HPP_CNM_CORE_MACHINE_SERVER_SERVER_HPP
#define HPP_CNM_CORE_MACHINE_SERVER_SERVER_HPP

#include "cnm/machine/machine.hpp"
#include "cnm/machine/server/server_logic.hpp"
#include "cnm/utils/thread_pool.hpp"

namespace Cnm {

class Server final : public Machine {
 public:
  Server(ServerLogic&&, size_t limit, HostInfo host_info,
         std::shared_ptr<Communicator> communicator)
      : Machine("server", limit, host_info, std::move(communicator)),
        thread_pool{} {}

  ~Server() override { stop(); }

  void start() override {
    // we initialize the thread_pool.
  }

  void stop() override {
    // delete all tasks.
    // signal to end all threads.
    // delete the thread_pool
  }

  void invoke() override {
    // send signal to continue the execution of thread_pool
  }

  void freeze() override {
    // send signal to stop the execution of the thread_pool.
  }

  size_t getCurrentServingAmount() const noexcept override { return 0; }

  result_t<MessageBatch> serve(MessageBatch) override {}

 private:
  std::unique_ptr<Utils::ThreadPool> thread_pool;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_SERVER_SERVER_HPP
