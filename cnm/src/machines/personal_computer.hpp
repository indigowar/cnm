#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_PERSONAL_COMPUTER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_PERSONAL_COMPUTER_HPP

#include <thread>

#include "connection/requester_ctx.hpp"
#include "machines/machine.hpp"

namespace Cnm::Machines {

class PC : public Machine {
 public:
  using Logic = std::function<void(std::shared_ptr<Machine::Interactor>&,
                                   const HostInfo&, std::stop_token)>;

  PC(HostInfo host, Logic logic,
     std::shared_ptr<Machine::Interactor> interactor)
      : Machine(host),
        logic_(logic),
        interactor_(interactor),
        // that's probably the weirdest way of doing, but still.
        // hope the shared_ptr& will go in right way.
        running_thread_(std::bind(logic_, interactor_, info_)) {}

  ~PC() = default;

  size_t getServingAmount() const noexcept override { return 0; }

  std::string_view getType() const noexcept override { return "PC"; }

  void terminate() override { running_thread_.request_stop(); }

 private:
  std::jthread running_thread_;

  Logic logic_;
  std::shared_ptr<Machine::Interactor> interactor_;
};

}  // namespace Cnm::Machines

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_PERSONAL_COMPUTER_HPP
