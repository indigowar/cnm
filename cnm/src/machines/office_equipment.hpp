#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_OFFICE_EQUIPMENT_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_OFFICE_EQUIPMENT_HPP

#include <condition_variable>
#include <future>

#include "connection/server_ctx.hpp"
#include "machine.hpp"

namespace Cnm::Machines {

class OfficeEquipment : public Machine {
 public:
  using Logic = std::function<void(HostInfo& info,
                                   std::unique_ptr<Connection::ServerCtx>&&)>;

  OfficeEquipment(HostInfo info, Logic logic)
      : Machine(info), logic_(logic), terminate_{} {}

  ~OfficeEquipment() { terminate(); }

  void terminate() override { terminate_ = true; }

  size_t getServingAmount() const noexcept override { return 1; }

 private:
  Logic logic_;

  std::condition_variable busy_;
  bool terminate_;
};

}  // namespace Cnm::Machines

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_OFFICE_EQUIPMENT_HPP
