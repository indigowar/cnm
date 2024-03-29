#include "server.hpp"

#include <spdlog/spdlog.h>

namespace Cnm {

Server::Server(std::unique_ptr<ServerLogic>&& logic, size_t limit,
               HostInfo host_info, std::unique_ptr<Communicator> communicator)
    : Machine(Server::Type, limit, host_info, std::move(communicator)),
      thread_pool{},
      is_accepting{},
      logic{std::move(logic)},
      status{NotInitialized} {}

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
  logic->reset();
  logic->init(host_info);
  thread_pool->startThreads();
  status = Status::Running;
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

  status = Status::Dead;
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

  status = Status::Running;
  is_accepting = true;
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
  status = Status::Freezed;
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
  ctx->abort();
}

void Server::addRequestToThreadPool(Cnm::ServerCtx&& ctx) {
  auto task = [this, c{std::move(ctx)}]() mutable {
    logic->execute(communicator, std::move(c));
  };
  thread_pool->enqueue(std::move(task));
}

Object::Status Server::getStatus() const noexcept { return status; }

}  // namespace Cnm
