#ifndef HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP
#define HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP

namespace Cnm {

class ServerLogic {
 public:
  virtual ~ServerLogic() = default;

  virtual void init() {}

  virtual void execute(ServerCtx&&) {}
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP
