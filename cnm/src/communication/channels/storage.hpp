#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP

#include <exception>
#include <future>
#include <queue>

#include "communication/channels/exceptions.hpp"

namespace cnm::communication {

/**
 * This class is internal channel storage @see channel
 *
 * It is not multi-thread safe.
 * You must have additional layer that manages the access to object of this
 * class.
 *
 * @tparam T
 */
template <class T>
class channel_storage {
 public:
  explicit channel_storage(size_t limit = 0) : m_limit{limit} {}

  ~channel_storage() { this->close(); }

  void push(T value) {
    throw_if_closed();
    if (has_expected()) {
      put_in_expected(value);
      return;
    }
    save(value);
  }

  std::future<T> pop() {
    this->throw_if_closed();
    if (has_saved()) {
      return from_saved();
    }
    return expect();
  }

  [[nodiscard]] size_t size() const noexcept { return m_saved.size(); }

  [[nodiscard]] size_t limit() const noexcept { return m_limit; }

  [[nodiscard]] bool has_limit() const noexcept { return m_limit != 0; }

  [[nodiscard]] bool empty() const noexcept { return m_saved.size() != 0; }

  [[nodiscard]] bool is_closed() const noexcept { return m_closed; }

  void close() {
    const auto exception =
        std::make_exception_ptr(exceptions::channel_closed_error());
    while (!m_expected.empty()) {
      auto promise = std::move(m_expected.front());
      m_expected.pop();
      promise.set_exception(exception);
    }
    m_closed = true;
  }

  channel_storage(const channel_storage<T>&) = delete;
  channel_storage<T>& operator=(const channel_storage<T>&) = delete;
  channel_storage(channel_storage<T>&&) noexcept = default;
  channel_storage& operator=(channel_storage<T>&&) noexcept = default;

 private:
  [[nodiscard]] bool has_saved() const noexcept { return !m_saved.empty(); }
  [[nodiscard]] bool has_expected() const noexcept {
    return !m_expected.empty();
  }

  void throw_if_closed() {
    if (m_closed) {
      throw exceptions::channel_closed_error();
    }
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

  std::queue<std::future<T>> m_saved;
  std::queue<std::promise<T>> m_expected;
  size_t m_limit;
  bool m_closed{};
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP
