#include "server_ctx.hpp"

#include "cnm/connection/connection.hpp"

using namespace Cnm;

ServerContext::ServerContext(std::shared_ptr<Connection> c,
                             Connections::ServerT srv)
    : connection{std::move(c)}, server_node(std::move(srv)) {}

ServerContext::~ServerContext() = default;

void ServerContext::accept() {
  // signals the Connection, that the server is ready to retrieve requests.
  connection->signalAccept();
}

std::future<result_t<MessageBatch>> ServerContext::getFutureRequest() {
  return std::async(std::launch::async, [this] {
    auto wait_result = connection->waitForServing();
    if (wait_result.isErr()) {
      return result_t<MessageBatch>::Err("failed to wait for Serving");
    }

    auto msgs = server_node->getBuffer();
    if (msgs.empty()) {
      return result_t<MessageBatch>::Err("the request is empty");
    }
    if (msgs.size() == 1) {
      return result_t<MessageBatch>::Ok(MessageBatch(std::move(msgs[0])));
    }

    return result_t<MessageBatch>::Ok(MessageBatch(std::move(msgs)));
  });
}

std::future<result_t<MessageBatch>> ServerContext::acceptAndGetFutureRequest() {
  accept();
  return getFutureRequest();
}

result_t<MessageBatch> ServerContext::getRequest() {
  auto future = getFutureRequest();
  future.wait();
  return future.get();
}

result_t<MessageBatch> ServerContext::acceptAndGetRequest() {
  accept();
  return getRequest();
}

void ServerContext::abort() { server_node->abort(); }

result_t<bool> ServerContext::respond(const MessageBatch& response) {
  if (!connection->isServing()) {
    return result_t<bool>::Err("connection is not serving");
  }

  if (response.containsOneMessage()) {
    server_node->sendBackward(response.getMessage());
  } else {
    for (auto m : response.getMessageList()) {
      server_node->sendBackward(std::move(m));
    }
  }

  connection->signalComplete();
  return result_t<bool>::Ok(true);
}

ServerContext::ServerContext(ServerContext&& ctx) noexcept
    : connection{std::move(ctx.connection)},
      server_node{std::move(ctx.server_node)} {}

ServerContext& ServerContext::operator=(ServerContext&& ctx) noexcept {
  server_node->abort();

  connection = std::move(ctx.connection);
  server_node = std::move(ctx.server_node);

  return *this;
}
