#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP

#include <string_view>

#include "communication/slave_ctx.hpp"

namespace Cnm::Machines {

class Machine {
 public:
  explicit Machine(std::string_view host) : host{host} {}

  virtual ~Machine() = default;

  // calling this method should start a termination process
  // so all threads that runner contains should be destroyed
  void terminate() noexcept { onTermination(); }

  [[nodiscard]] std::string_view getHost() const noexcept { return host; }

  /**
   * @brief serve the given connection(task)
   *
   * Currently as an argument we give serve_object_mock
   * because the connections are not ready and
   * there's no need in specifying of runner's behavior.
   */
  virtual void serve(Communication::slave_ctx&&) noexcept = 0;

 protected:
  // this method provides the logic of termination the runner
  virtual void onTermination() noexcept = 0;

 private:
  // this is the value contains an address
  // that, in network, is given to every node
  std::string_view host;
};

}  // namespace Cnm::Machines

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_RUNNER_HPP