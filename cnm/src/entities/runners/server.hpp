#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP

#include <memory>

#include "communication/slave_ctx.hpp"
#include "entities/runners/runner.hpp"
#include "utils/thread_pool.hpp"

namespace cnm::entities::runners {

class server : public base_runner {
 public:
  using logic_t = std::function<void(communication::slave_ctx&&)>;

  server(std::string_view host, size_t concurrent_capabilities,
         logic_t handler);

  ~server();

  virtual void serve(communication::slave_ctx&&) noexcept = 0;

 private:
  logic_t m_logic;
  utils::thread_pool m_pool;
};

}  // namespace cnm::entities::runners

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
