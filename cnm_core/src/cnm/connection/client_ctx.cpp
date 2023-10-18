#include "client_ctx.hpp"

#include "cnm/connection/connection.hpp"

using namespace Cnm;

ClientContext::ClientContext(std::shared_ptr<Connection> con,
                             Connections::ClientT client)
    : connection{std::move(con)}, client_node{std::move(client)} {}

ClientContext::~ClientContext() = default;

ClientContext::ClientContext(ClientContext&& ctx) noexcept
    : connection{std::move(ctx.connection)},
      client_node{std::move(ctx.client_node)} {}

ClientContext& ClientContext::operator=(ClientContext&& ctx) noexcept {
  client_node->abort();

  connection = std::move(ctx.connection);
  client_node = std::move(ctx.client_node);

  return *this;
}

std::future<result_t<bool>> ClientContext::getFutureUntilAccepted() {
  return std::async(std::launch::async, [this] {
    auto wait_result = connection->waitForRequesting();
    if (wait_result.isErr()) {
      return result_t<bool>::Err("failed to accept");
    }
    return result_t<bool>::Ok(true);
  });
}

result_t<bool> ClientContext::waitUntilAccepted() {
  auto result = getFutureUntilAccepted();
  result.wait();
  return result.get();
}

result_t<bool> ClientContext::request(MessageBatch&& req) {
  if (!connection->isRequesting()) {
    return result_t<bool>::Err("connection is not expecting a request");
  }

  if (req.containsOneMessage()) {
    client_node->sendForward(req.getMessage());
  } else {
    for (auto m : req.getMessageList()) {
      client_node->sendForward(std::move(m));
    }
  }
  connection->signalStopRequest();

  return result_t<bool>::Ok(true);
}

std::future<result_t<MessageBatch>> ClientContext::getFutureResponse() {
  return std::async(std::launch::async, [this] {
    auto wait_result = connection->waitForComplete();
    if (wait_result.isErr()) {
      return result_t<MessageBatch>::Err(wait_result.unwrapErr());
    }

    auto msgs = client_node->getBuffer();
    if (msgs.empty()) {
      return result_t<MessageBatch>::Err("the response is empty");
    }
    if (msgs.size() == 1) {
      return result_t<MessageBatch>::Ok(MessageBatch(std::move(msgs[0])));
    }

    return result_t<MessageBatch>::Ok(MessageBatch(std::move(msgs)));
  });
}

result_t<MessageBatch> ClientContext::getResponse() {
  auto future = getFutureResponse();
  future.wait();
  return future.get();
}

void ClientContext::abort() { client_node->abort(); }
