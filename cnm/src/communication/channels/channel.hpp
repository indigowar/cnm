#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP

#include <future>
#include <mutex>
#include <shared_mutex>

#include "communication/channels/exceptions.hpp"
#include "communication/channels/storage.hpp"

namespace cnm::communication {

template <class package_t>
class channel final {
 public:
  static channel<package_t> make_with_limit(size_t limit_value) {
    return channel<package_t>(channel_storage<package_t>(limit_value));
  }

  static channel<package_t> make_unbuffered() {
    return channel<package_t>(channel_storage<package_t>());
  }

  explicit channel(channel_storage<package_t>&& storage)
      : m_storage(std::move(storage)) {}

  ~channel() = default;

  [[nodiscard]] bool is_buffered() const noexcept {
    std::shared_lock lock(m_mutex);
    return m_storage.has_limit();
  }

  [[nodiscard]] bool is_closed() const noexcept {
    std::shared_lock lock(m_mutex);
    return m_storage.is_closed();
  }

  [[nodiscard]] size_t get_limit() const {
    std::shared_lock lock(m_mutex);
    if (!m_storage.has_limit()) {
      throw exceptions::channel_unbuffered_error();
    }
    return m_storage.limit();
  }

  [[nodiscard]] size_t get_amount_of_contained() const noexcept {
    std::shared_lock lock(m_mutex);
    return m_storage.size();
  }

  void close() {
    std::unique_lock lock(m_mutex);
    if (m_storage.is_closed()) {
      throw exceptions::channel_closed_error();
    }
    m_storage.close();
  }

  // write method
  channel<package_t>& operator<<(package_t value) {
    std::unique_lock lock(m_mutex);
    m_storage.push(value);
    return *this;
  }

  // sync read
  channel<package_t>& operator>>(package_t& value) {
    auto future = read_from_storage();
    future.wait();
    value = future.get();
    return *this;
  }

  // async read
  channel<package_t>& operator>>(std::future<package_t>& value) {
    value = std::move(read_from_storage());
    return *this;
  }

  channel(const channel<package_t>&) = delete;
  channel& operator=(const channel<package_t>&) = delete;

  channel(channel<package_t>&&) = delete;
  channel& operator=(channel<package_t>&&) = delete;

 private:
  std::future<package_t> read_from_storage() {
    std::unique_lock lock(m_mutex);
    return m_storage.pop();
  }

  channel_storage<package_t> m_storage;

  mutable std::shared_mutex m_mutex;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP