#ifndef HPP_CNM_CORE_MACHINE_MACHINE_HPP
#define HPP_CNM_CORE_MACHINE_MACHINE_HPP

#include <string_view>
namespace Cnm {

struct ConnectionEntity {};

class Machine {
 public:
  Machine(std::string_view name) : name_{name} {}

  virtual void serve(ConnectionEntity) = 0;

  virtual bool is_running() const noexcept = 0;
  virtual bool is_serving() const noexcept = 0;
  virtual bool is_frozen() const noexcept = 0;

  std::string_view name() const noexcept { return name_; }

 private:
  std::string_view name_;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_MACHINE_HPP
