#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP

#include <future>
#include <list>
#include <mutex>
#include <shared_mutex>

namespace cnm::communication {

template <class package_t>
class channel final {
 public:
  class unbuffered_error : public std::exception {};

  class closed_channel_error : public std::exception {};

  class overflowed_channel_error : public std::exception {};

  static channel<package_t> make_with_limit(size_t limit_value);

  static channel<package_t> make_unbuffered();

  bool is_buffered() const noexcept;
  bool is_closed() const noexcept;

  size_t get_limit() const;

  size_t get_amount_of_contained() const noexcept;

  void close();

  channel<package_t>& operator<<(package_t value) {}

  channel<package_t>& operator>>(package_t& value) {}

 private:
  struct storage_t {
    virtual ~storage_t() = default;
    virtual void push(package_t) = 0;
    virtual std::future<package_t> pop() = 0;
    virtual size_t capacity() const noexcept = 0;
    virtual size_t current() const noexcept = 0;
  };

  struct buffered_storage_t : public storage_t {
   public:
    explicit buffered_storage_t(size_t max_size) : m_max_size(max_size) {}

    void push(package_t package) override {
      std::unique_lock lock(m_mutex);
      if (m_list.size() >= m_max_size) {
        throw overflowed_channel_error();
      }
      std::promise<package_t> promise;
      m_list.push_back(std::move(promise.get_future()));
      promise.set_value(package);
    }

    std::future<package_t> pop() override {}

    size_t capacity() const noexcept override {
      std::shared_lock lock(m_mutex);
      return m_max_size;
    }

    size_t current() const noexcept override {
      std::shared_lock lock(m_mutex);
      return m_list.size();
    }

   private:
    std::list<std::future<package_t>> m_list;
    std::list<std::promise<package_t>> m_packages;
    size_t m_max_size;
    std::shared_mutex m_mutex;
  };

  struct unbuffered_storage_t : public storage_t {};
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP