#ifndef HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP
#define HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP

#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>

#include "cnm/connection/internal/client_node.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

class Connection;

class ClientContext final {
 public:
  explicit ClientContext(Connection*, std::shared_ptr<Connections::ClientNode>);

  ClientContext(ClientContext&& ctx) noexcept;

  // waitUntilAccepted() - waits until the Connection is accepted by server.
  result_t<bool> waitUntilAccepted();

  // sendRequest(MessageBatch&&) - sends request to the server.
  void sendRequest(MessageBatch&&);

  // getResponse() - returns the future which will have result.
  std::future<result_t<MessageBatch>> getResponse();

  // waitAndGetResponse() - waits inside the function and returns result.
  result_t<MessageBatch> waitAndGetResponse();

  // abort() - end the connection immediately.
  void abort();

  [[nodiscard]] bool isAborted() const noexcept;

  [[nodiscard]] bool isServing() const noexcept;

  [[nodiscard]] bool isRequesting() const noexcept;

 private:
  Connection* connection;
  std::shared_ptr<Connections::ClientNode> client_node;

  mutable std::mutex mutex;
  std::condition_variable cond_var;
};

using ClientCtx = std::unique_ptr<ClientContext>;

}  // namespace Cnm

#endif  // HPP_CNM_CORE_CONNECTION_CLIENT_CTX_HPP
