#include "client_ctx.hpp"

#include <spdlog/spdlog.h>

#include "cnm/connection/connection.hpp"

namespace Cnm {

ClientContext::ClientContext(
    Cnm::Connection *connection,
    std::shared_ptr<Connections::ClientNode> client_node)
    : connection{connection}, client_node{std::move(client_node)} {}

ClientContext::ClientContext(ClientContext &&ctx) noexcept
    : connection{ctx.connection}, client_node{std::move(ctx.client_node)} {}

result_t<bool> ClientContext::waitUntilAccepted() {
  {
    auto lock = connection->makeLock();
    cond_var.wait(lock, [this] {
      return connection->isAborted() || connection->isRequesting();
    });
  }

  if (connection->isAborted()) {
    return result_t<bool>::Err("connection was aborted");
  }
  return result_t<bool>::Ok(true);
}

void ClientContext::sendRequest(Cnm::MessageBatch &&batch) {
  if (!connection->isRequesting()) {
    return;
  }

  if (batch.containsMessageList()) {
    for (auto &i : batch.getMessageList()) {
      client_node->sendForward(std::move(i));
    }
  } else {
    client_node->sendForward(batch.getMessage());
  }
}

std::future<result_t<MessageBatch>> ClientContext::getResponse() {
  return std::async([this] {
    {
      auto lock = connection->makeLock();
      cond_var.wait(lock, [this] { return !connection->isServing(); });
    }

    if (connection->isAborted()) {
      spdlog::warn(
          "ClientContext::getResponse::async - connection was aborted");
    }

    auto buffer = client_node->getBuffer();

    if (buffer.empty()) {
      return result_t<MessageBatch>::Err(
          "connection was aborted before sending any data");
    }

    if (buffer.size() == 1) {
      auto value = buffer.at(0);
      MessageBatch batch(std::move(value));
      return result_t<MessageBatch>::Ok(std::move(batch));
    }

    MessageBatch batch(std::move(buffer));
    return result_t<MessageBatch>::Ok(std::move(batch));
  });
}

result_t<MessageBatch> ClientContext::waitAndGetResponse() {
  auto result = getResponse();
  result.wait();
  return result.get();
}

void ClientContext::abort() { client_node->abort(); }

bool ClientContext::isAborted() const noexcept {
  return connection->isAborted();
}

bool ClientContext::isServing() const noexcept {
  return connection->isServing();
}

bool ClientContext::isRequesting() const noexcept {
  return connection->isRequesting();
}

}  // namespace Cnm