#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP

#include <memory>

#include "communication/slave_ctx.hpp"
#include "machine.hpp"
#include "utils/thread_pool.hpp"

namespace Cnm::Machines {

class Server : public Machine {
 public:
  using logic_t = std::function<void(Communication::slave_ctx&&)>;

  Server(std::string_view host, size_t concurrent_capabilities,
         logic_t handler);

  ~Server() noexcept;

  virtual void serve(Communication::slave_ctx&&) noexcept override;

 private:
  void onTermination() noexcept final;

  const logic_t logic;
  utils::thread_pool pool;
};

}  // namespace Cnm::Machines

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
