#include "machines/server.hpp"

namespace Cnm::Machines {

Server::Server(HostInfo info, LogicT logic, size_t workers)
    : Machine(info), logic_{std::move(logic)}, pool_(workers) {}

// Server::Server(HostInfo info, size_t concurrent_capabilies, Logic logic)
//     : Machine(info), pool_(concurrent_capabilies), logic_{logic} {}
//
// Server::~Server() { terminate(); }
//
// void Server::serve(std::unique_ptr<Connection::ServerCtx>&& ctx) {
//   auto task = [this, &ctx] { logic_(info_, std::move(ctx)); };
//
//   pool_.push(task);
// }
//
// std::string_view Server::getType() const noexcept { return "Server"; }
//
// void Server::terminate() { pool_.stop(); }

}  // namespace Cnm::Machines
