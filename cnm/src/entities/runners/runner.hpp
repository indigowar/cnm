#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP

#include <string_view>

#include "communication/slave_ctx.hpp"

namespace cnm::entities::runners {

/**
 * @brief base_runner is a base class for all runners that exists in network.
 */
class base_runner {
 public:
  explicit base_runner(std::string_view host) : m_host{host} {}

  virtual ~base_runner() = default;

  // calling this method should start a termination process
  // so all threads that runner contains should be destroyed
  void terminate() noexcept { on_termination(); }

  [[nodiscard]] std::string_view host() const noexcept { return m_host; }

  /**
   * @brief serve the given connection(task)
   *
   * Currently as an argument we give serve_object_mock
   * because the connections are not ready and
   * there's no need in specifying of runner's behavior.
   */
  virtual void serve(communication::slave_ctx&&) noexcept = 0;

 protected:
  // this method provides the logic of termination the runner
  virtual void on_termination() noexcept = 0;

 private:
  // this is the value contains an address
  // that, in network, is given to every node
  std::string_view m_host;
};

}  // namespace cnm::entities::runners

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP