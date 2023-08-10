#ifndef HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP
#define HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP

namespace Cnm {

class ServerLogic {
 public:
  void init() {}
  result_t<MessageBatch> execute(MessageBatch msg) {
    return result_t<MessageBatch>::Ok(std::move(msg));
  }
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP
