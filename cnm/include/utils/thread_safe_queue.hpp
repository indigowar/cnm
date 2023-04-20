#ifndef HPP_CNM_LIB_UTILS_SAFE_QUEUE_HPP
#define HPP_CNM_LIB_UTILS_SAFE_QUEUE_HPP
/**
 * @file This file contains an interface for thread-safe queue
 */

#include <mutex>
#include <optional>
#include <queue>
#include <shared_mutex>

namespace cnm::utils {

/**
 * @brief This is a thread-safe implementations of std::queue
 */
template <class T>
class thread_safe_queue final : protected std::queue<T> {
 public:
  thread_safe_queue() = default;

  ~thread_safe_queue() { clear(); }
  /**
   * @brief returns true if the queue is empty
   *
   * @returns is the queue empty
   */
  bool empty() const noexcept {
    std::shared_lock lock{m_mutex};
    return std::queue<T>::empty();
  }

  /**
   * @returns the number of elements in the queue
   */
  size_t size() const noexcept {
    std::shared_lock lock{m_mutex};
    return std::queue<T>::size();
  }

  /**
   * @brief empties the queue(deletes all it's elements)
   */
  void clear() noexcept {
    std::unique_lock lock{m_mutex};
    while (!std::queue<T>::empty()) {
      std::queue<T>::pop();
    }
  }

  /**
   * @brief adds a new value to the end of queue
   */
  void push(const T& value) noexcept {
    std::unique_lock lock{m_mutex};
    std::queue<T>::push(value);
  }

  void pop() noexcept {
    std::unique_lock lock(m_mutex);
    std::queue<T>::pop();
  }

  T front() const noexcept {
    std::shared_lock lock(m_mutex);
    return std::queue<T>::front();
  }

  thread_safe_queue(const thread_safe_queue&) = delete;
  thread_safe_queue(thread_safe_queue&&) = delete;
  thread_safe_queue& operator=(const thread_safe_queue&) = delete;
  thread_safe_queue& operator=(thread_safe_queue&&) = delete;

 private:
  mutable std::shared_mutex m_mutex;
};
}  // namespace cnm::utils

#endif  // HPP_CNM_LIB_UTILS_SAFE_QUEUE_HPP