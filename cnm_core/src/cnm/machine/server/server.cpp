#include "server.hpp"

#include <spdlog/spdlog.h>

namespace Cnm {

Server::Server(ServerLogic&& logic, size_t limit, HostInfo host_info,
               std::shared_ptr<Communicator> communicator)
    : Machine(Server::Type, limit, host_info, std::move(communicator)),
      thread_pool{},
      is_accepting{},
      logic{logic} {}

Server::~Server() { stop(); }

void Server::start() {
  auto lock = makeLock();

  if (thread_pool) {
    spdlog::warn("Server::start(): the thread pool is already created.");
    return;
  }

  if (is_accepting) {
    spdlog::warn(
        "Server::start(): the server is already accepting the requests.");
    return;
  }

  is_accepting = true;

  thread_pool = std::make_unique<Utils::ThreadPool>(getServingLimit());
  thread_pool->startThreads();
}

void Server::stop() {
  auto lock = makeLock();

  if (!thread_pool) {
    spdlog::warn("Server::stop(): the thread pool is dead.");
    return;
  }

  is_accepting = false;
  thread_pool->stopThreads();
  thread_pool.reset(nullptr);
}

void Server::invoke() {
  auto lock = makeLock();

  if (!thread_pool) {
    spdlog::warn("Server::invoke(): the thread pool is dead.");
    return;
  }

  if (is_accepting) {
    spdlog::warn(
        "Server::invoke() called when Server does accept requests already.");
    return;
  }
}

void Server::freeze() {
  auto lock = makeLock();

  if (!thread_pool) {
    spdlog::warn("Server::invoke(): the thread pool is dead.");
    return;
  }

  if (!is_accepting) {
    spdlog::warn(
        "Server::freeze() called when Server does not accept requests.");
    return;
  }

  is_accepting = false;
}

size_t Server::getCurrentServingAmount() const noexcept {
  if (!thread_pool) return 0;
  return thread_pool->getNumBusyWorkers();
}

void Server::serve(ServerCtx&& ctx) {
  auto lock = makeLock();

  if (is_accepting) {
    addRequestToThreadPool(std::move(ctx));
    return;
  }

  spdlog::warn("Server::serve() called when Server does not accept requests.");
  // ctx->abort();
}

void Server::addRequestToThreadPool(Cnm::ServerCtx&& ctx) {
  auto task = [this, &ctx] { logic.execute(std::move(ctx)); };
  thread_pool->enqueue(task);
}

}  // namespace Cnm
