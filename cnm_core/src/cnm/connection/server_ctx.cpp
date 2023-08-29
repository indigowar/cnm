#include "server_ctx.hpp"

#include "cnm/connection/connection.hpp"

namespace Cnm {

ServerContext::ServerContext(
    Cnm::Connection *connection,
    std::shared_ptr<Connections::ServerNode> server_node)
    : connection{connection}, server_node{std::move(server_node)} {}

ServerContext::ServerContext(Cnm::ServerContext &&other) noexcept
    : connection{other.connection}, server_node{std::move(other.server_node)} {}

ServerContext &ServerContext::operator=(ServerContext &&other) noexcept {
  connection = other.connection;
  server_node = std::move(other.server_node);

  return *this;
}

std::future<result_t<MessageBatch>> ServerContext::acceptRequest() {
  auto lock = connection->makeLock();

  if (connection->isServing() || connection->isAborted()) {
    std::promise<result_t<MessageBatch>> promise;
    promise.set_value(result_t<MessageBatch>::Err(
        "requesting stage of connection is already passed."));
    return promise.get_future();
  }
}

void ServerContext::abort() { server_node->abort(); }

void ServerContext::sendResponse(Cnm::MessageBatch &&response) {
  if (!connection->isServing()) {
    return;
  }

  if (response.containsOneMessage()) {
    server_node->sendBackward(std::move(response.getMessage()));
    return;
  }

  for (auto i : response.getMessageList()) {
    server_node->sendBackward(std::move(i));
  }
}

}  // namespace Cnm