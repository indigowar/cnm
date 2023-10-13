#ifndef HPP_CNM_CORE_MACHINE_PERSONAL_COMPUTER_PERSONAL_COMPUTER_LOGIC
#define HPP_CNM_CORE_MACHINE_PERSONAL_COMPUTER_PERSONAL_COMPUTER_LOGIC

#include "cnm/machine/communicator.hpp"

namespace Cnm {

class PersonalComputerLogic {
 public:
  virtual ~PersonalComputerLogic() = default;

  virtual void init() {}

  virtual void execute(std::unique_ptr<Communicator>& communicator) {}
};

class RandomPersonalComputerLogic final : public PersonalComputerLogic {
 public:
  void init() override;

  void execute(std::unique_ptr<Communicator>&) override;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_PERSONAL_COMPUTER_PERSONAL_COMPUTER_LOGIC
