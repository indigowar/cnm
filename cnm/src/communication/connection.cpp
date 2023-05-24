#include "connection.hpp"

#include <random>

#include "master_ctx.hpp"
#include "slave_ctx.hpp"

using namespace Cnm::Communication;

static int get_random() {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_int_distribution dist(1, 10000);

  return dist(mt);
}

connection::connection(size_t net_speed)
    : m_net_speed{net_speed},
      m_initiator(get_random()),
      m_server(get_random()),
      m_master_ctx{std::unique_ptr<context>(
          new Cnm::Communication::master_ctx(*this, m_initiator))},
      m_slave_ctx{std::unique_ptr<context>(
          new Cnm::Communication::slave_ctx(*this, m_server))} {}

size_t connection::net_speed() const noexcept { return m_net_speed; }

std::unique_ptr<context> connection::master_ctx() noexcept {
  if (m_master_ctx) {
    auto result = std::move(m_master_ctx);
    m_master_ctx = nullptr;
    return std::move(result);
  }
  return nullptr;
}

std::unique_ptr<context> connection::slave_ctx() noexcept {
  if (m_slave_ctx) {
    auto result = std::move(m_slave_ctx);
    m_slave_ctx = nullptr;
    return std::move(result);
  }
  return nullptr;
}