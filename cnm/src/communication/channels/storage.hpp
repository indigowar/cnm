#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP

#include <exception>
#include <future>
#include <queue>

#include "communication/channels/exceptions.hpp"

namespace cnm::communication {

template <class T>
class channel_storage {
 public:
  explicit channel_storage(size_t limit = 0) : m_limit{limit} {}

  ~channel_storage() {
    const auto exception =
        std::make_exception_ptr(exceptions::channel_closed_error());
    for (std::promise<T>& promise : m_expected) {
      promise.set_exception(exception);
    }
  }

  void push(T value) {
    std::unique_lock lock(m_mutex);
    if (has_expected()) {
      put_in_expected(value);
      return;
    }
    save(value);
  }

  std::future<T> pop() {
    std::unique_lock lock(m_mutex);
    if (has_saved()) {
      return from_saved();
    }
    return expect();
  }

  [[nodiscard]] size_t size() const noexcept {
    std::unique_lock lock(m_mutex);
    return m_saved.size();
  }

  channel_storage(const channel_storage<T>&) = delete;
  channel_storage<T>& operator=(const channel_storage<T>&) = delete;

  channel_storage(channel_storage<T>&&) = delete;
  channel_storage& operator=(channel_storage<T>&&) = delete;

 private:
  [[nodiscard]] bool has_saved() const noexcept { return !m_saved.empty(); }
  [[nodiscard]] bool has_expected() const noexcept {
    return !m_expected.empty();
  }

  std::future<T> expect() {
    std::promise<T> promise{};
    auto future = promise.get_future();
    m_expected.push(std::move(promise));
    return future;
  }

  std::future<T> from_saved() {
    auto future = std::move(m_saved.front());
    m_saved.pop();
    return future;
  }

  void save(T value) {
    if (m_saved.size() >= m_limit && has_limit()) {
      throw exceptions::channel_overflowed_error(m_limit);
    }
    std::promise<T> promise{};
    auto future = promise.get_future();
    promise.set_value(value);
    m_saved.push(std::move(future));
  }

  void put_in_expected(T value) {
    auto promise = std::move(m_expected.front());
    m_expected.pop();
    promise.set_value(value);
  }

  bool has_limit() const noexcept { return m_limit != 0; }

  std::queue<std::future<T>> m_saved;
  std::queue<std::promise<T>> m_expected;
  std::mutex m_mutex;
  size_t m_limit;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP
