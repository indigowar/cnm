#ifndef HPP_CNM_CORE_UTILS_ONE_TIME_BUILDER_HPP
#define HPP_CNM_CORE_UTILS_ONE_TIME_BUILDER_HPP

#include <mutex>

#include "cnm/utils/result.hpp"
namespace Utils {

template <typename Object>
class OneTimeBuilder final {
 public:
  explicit OneTimeBuilder() : is_created{} {}

  template <typename... Args>
  result_t<Object> build(Args... args) noexcept {
    std::unique_lock lock(mutex);
    if (is_created) {
      return result_t<Object>::Err("object has been already created");
    }
    is_created = true;
    return result_t<Object>::Ok(Object(std::forward<Args>(args)...));
  }

 private:
  std::mutex mutex;
  bool is_created;
};

}  // namespace Utils

#endif  // HPP_CNM_CORE_UTILS_ONE_TIME_BUILDER_HPP
