#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_PERSONAL_COMPUTER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_PERSONAL_COMPUTER_HPP

#include "connection/requester_ctx.hpp"
#include "machines/machine.hpp"

namespace Cnm::Machines {

class PC : public Machine {
 public:
  struct Interactor final {
    bool isConnected();
    std::unique_ptr<Connection::RequesterCtx> makeConnection();
    void disconnect();
  };

  using Logic = std::function<void(PC::Interactor)>;

 private:
};

}  // namespace Cnm::Machines

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_PERSONAL_COMPUTER_HPP
