#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP

#include <memory>

#include "communication/slave_ctx.hpp"
#include "entities/runners/runner.hpp"
#include "utils/thread_pool.hpp"

namespace cnm::entities::runners {

class server : public base_runner {
 public:
  class logic {
   public:
    virtual void operator()(server&, communication::slave_ctx& ctx) = 0;
  };

  [[maybe_unused]] server(std::string_view host, std::unique_ptr<logic> l)
      : base_runner(host), m_handler(std::move(l)) {}

  ~server() override = default;

  [[maybe_unused]] void serve(communication::slave_ctx&&) noexcept override {}


 private:
  std::unique_ptr<logic> m_handler;

  utils::thread_pool m_pool;
};

}  // namespace cnm::entities::runners

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
