#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP

#include <memory>

#include "connection/server_ctx.hpp"
#include "machines/machine.hpp"
#include "utils/thread_pool.hpp"

namespace Cnm::Machines {

class Server : public Machine {
 public:
  using LogicT = std::function<void(const HostInfo&, Interactor*,
                                    std::unique_ptr<Connection::ServerCtx>&&)>;

  Server(HostInfo info, LogicT logic, size_t workers);

  size_t getServingAmount() const noexcept override;

  std::string_view getType() const noexcept override { return "server"; }

  void serve(std::unique_ptr<Connection::ServerCtx>&&) override;

  void terminate() override;

  void start() override;
  void stop() override;

 private:
  LogicT logic_;
  Utils::ThreadPool pool_;
};

// class Server : public Machine {
//  public:
//   using Logic = std::function<void(const HostInfo&,
//                                    std::unique_ptr<Connection::ServerCtx>&&)>;
//
//   Server(HostInfo info, size_t max_connection, Logic logic);
//
//   ~Server();
//
//   std::string_view getType() const noexcept override;
//
//   void serve(std::unique_ptr<Connection::ServerCtx>&&) override;
//
//   void terminate() override;
//
//  private:
//   Logic logic_;
//   utils::thread_pool pool_;
// };
//
}  // namespace Cnm::Machines

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
