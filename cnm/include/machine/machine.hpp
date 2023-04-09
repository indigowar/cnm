#ifndef HPP_CNM_LIB_MACHINE_MACHINE_HPP
#define HPP_CNM_LIB_MACHINE_MACHINE_HPP

#include <string_view>

namespace cnm::machine {
/**
 * Machine is the base class for all machines in network
 * (PCs, Office Equipment, Servers)
 */
class Machine {
 public:
  explicit Machine(std::string_view host) : m_host(host) {}

  virtual ~Machine() { on_shutdown(); }

  std::string_view host() const noexcept { return m_host; }

  void host(std::string_view host) noexcept { m_host = host; }

  /**
   * Add here connection retrievers
   */

 protected:
  virtual void on_shutdown() {}

 private:
  std::string_view m_host;
};

}  // namespace cnm::machine

#endif  // HPP_CNM_LIB_MACHINE_MACHINE_HPP