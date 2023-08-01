#ifndef HPP_CNM_CORE_UTILS_SLEEP_WRAPPER_HPP
#define HPP_CNM_CORE_UTILS_SLEEP_WRAPPER_HPP

#include <chrono>
#include <thread>

namespace Utils {

class SleepWrapper {
 public:
  virtual ~SleepWrapper() = default;

  virtual void sleepFor(const std::chrono::milliseconds& ms) const {
    std::this_thread::sleep_for(ms);
  }
};

}  // namespace Utils

#endif  // HPP_CNM_CORE_UTILS_SLEEP_WRAPPER_HPP
