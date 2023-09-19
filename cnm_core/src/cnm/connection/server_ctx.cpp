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
  return std::async([this] {
    {
      auto lock = connection->makeLock();

      if (connection->isAborted()) {
        return result_t<MessageBatch>::Err("connection is aborted");
      }

      // Have to wait until the requesting stage is ended.
      // It could ends in two state - serving(working) and aborted.
      cond_var.wait(lock, [this] {
        return connection->isServing() || connection->isAborted();
      });

      if (connection->isAborted()) {
        return result_t<MessageBatch>::Err("connection is aborted");
      }
    }

    auto buffer = server_node->getBuffer();

    if (buffer.empty()) {
      connection->abort();
      return result_t<MessageBatch>::Err(
          "connection is aborted due to the empty request");
    }

    return result_t<MessageBatch>::Ok(
        buffer.size() == 1 ? MessageBatch(std::move(buffer.at(0)))
                           : MessageBatch(std::move(buffer)));
  });
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
