#ifndef HPP_CNM_CORE_MACHINE_SERVER_SERVER_HPP
#define HPP_CNM_CORE_MACHINE_SERVER_SERVER_HPP

#include "cnm/machine/machine.hpp"
#include "cnm/machine/server/server_logic.hpp"
#include "cnm/utils/thread_pool.hpp"

namespace Cnm {

// Server - is a machine that serves something to clients.
class Server final : public Machine {
 public:
  Server(ServerLogic&& logic, size_t limit, HostInfo host_info,
         std::shared_ptr<Communicator> communicator);

  ~Server() override;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  size_t getCurrentServingAmount() const noexcept override;

  result_t<MessageBatch> serve(MessageBatch msg) override;

  static constexpr std::string_view Type = "server";

 private:
  std::future<result_t<MessageBatch>> addRequestToThreadPool(MessageBatch);

  // thread_pool is where the serving logic executes in.
  std::unique_ptr<Utils::ThreadPool> thread_pool;

  // is_accepting does the server accepts new requests.
  bool is_accepting;

  // ServerLogic object contains a custom logic for servers.
  ServerLogic logic;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_SERVER_SERVER_HPP
