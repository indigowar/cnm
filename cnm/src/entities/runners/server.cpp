#include "entities/runners/server.hpp"

using namespace cnm::entities::runners;

server::server(std::string_view host, size_t concurrent_capabilities,
               logic_t handler)
    : base_runner(host),
      m_pool(concurrent_capabilities),
      m_logic(std::move(handler)) {}

server::~server() noexcept { m_pool.stop(); }

void server::serve(communication::slave_ctx &&ctx) noexcept {
  auto task = [this, &ctx] {
    m_logic(std::move(ctx));
  };
  m_pool.push(task);
}
