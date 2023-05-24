#include "machines/server.hpp"

using namespace Cnm::Machines;

Server::Server(std::string_view host, size_t concurrent_capabilities,
               logic_t handler)
    : Machine(host), pool(concurrent_capabilities), logic(std::move(handler)) {}

Server::~Server() noexcept { onTermination(); }

void Server::serve(Communication::slave_ctx &&ctx) noexcept {
  auto task = [this, &ctx] { logic(std::move(ctx)); };
  pool.push(task);
}

void Server::onTermination() noexcept { pool.stop(); }
