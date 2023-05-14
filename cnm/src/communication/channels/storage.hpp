#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP

#include <exception>
#include <future>
#include <queue>

namespace cnm::communication {

template <class T>
class channel_internal_storage {
 public:
  struct storage_is_overflowed : public std::exception {};

  explicit channel_internal_storage(size_t limit) : m_limit{limit} {}

  ~channel_internal_storage() = default;

  void push(T value) {
    std::unique_lock lock(m_mutex);
    if (m_saved.size() + m_expected.size() >= m_limit) {
      throw storage_is_overflowed();
    }
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
    if (m_saved.size() + m_expected.size() >= m_limit) {
      throw storage_is_overflowed();
    }
    return expect();
  }

  [[nodiscard]] size_t size() const noexcept {
    return m_saved.size() + m_expected.size();
  }

  channel_internal_storage(const channel_internal_storage<T>&) = delete;
  channel_internal_storage<T>& operator=(const channel_internal_storage<T>&) =
      delete;

  channel_internal_storage(channel_internal_storage<T>&&) = delete;
  channel_internal_storage& operator=(channel_internal_storage<T>&&) = delete;

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
  std::mutex m_mutex;
  size_t m_limit;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_STORAGE_HPP
