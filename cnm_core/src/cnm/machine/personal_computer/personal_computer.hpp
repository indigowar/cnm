#ifndef HPP_CNM_CORE_MACHINE_PERSONAL_COMPUTER_PERSONAL_COMPUTER_HPP
#define HPP_CNM_CORE_MACHINE_PERSONAL_COMPUTER_PERSONAL_COMPUTER_HPP

#include <condition_variable>
#include <memory>
#include <thread>
#include <utility>

#include "cnm/core/message.hpp"
#include "cnm/machine/machine.hpp"
#include "cnm/machine/personal_computer/personal_computer_logic.hpp"
#include "cnm/utils/result.hpp"

namespace Cnm {

class PersonalComputer final : public Machine {
 public:
  PersonalComputer(PersonalComputerLogic&&, HostInfo,
                   std::unique_ptr<Communicator>&&);

  ~PersonalComputer() override;

  [[nodiscard]] size_t getCurrentServingAmount() const noexcept override;

  void serve(ServerCtx&& ctx) override;

  void start() override;

  void stop() override;

  void invoke() override;

  void freeze() override;

  Object::Status getStatus() const noexcept override;

  static constexpr std::string_view Type = "personal_computer";

 private:
  void threadRunner(std::stop_token);

  PersonalComputerLogic logic;

  std::unique_ptr<std::jthread> thread;
  std::condition_variable cond_var;
  bool continue_execution;

  Object::Status status;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_PERSONAL_COMPUTER_PERSONAL_COMPUTER_HPP
