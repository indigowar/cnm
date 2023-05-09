#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP

#include <condition_variable>
#include <future>
#include <mutex>
#include <optional>
#include <queue>

namespace cnm::communication {

/**
 * @brief channel class is the tool to interprocess communication.
 * @tparam T - the type of the package
 */
template <class T>
class channel {
  virtual ~channel() = default;

  /**
   * @brief write the value to the channel
   */
  virtual void write(T) = 0;

  /**
   * @brief read the value from the channel
   */
  virtual std::future<T> read() = 0;

  /**
   *
   * @return true if the channel is closed now
   */
  [[nodiscard]] virtual bool is_closed() const noexcept = 0;

  /**
   * @brief close the channel(can't read or write)
   */
  virtual void close() = 0;

  /**
   *
   * @return the max number of packages it can send
   */
  [[nodiscard]] virtual size_t limit() const noexcept = 0;
};

template <class package_t, size_t LIMIT>
class channel_t : public channel<package_t> {
 public:
  explicit channel_t() = default;

  ~channel_t() { close(); }

  void write(package_t package) override {
    std::unique_lock lock(mutex_);
    throw_exception_if_closed();

    if (has_pending()) {
      write_in_pending(package);
      return;
    }
    write_in_done(package);
  }

  std::future<package_t> read() override {
    std::unique_lock lock(mutex_);
    throw_exception_if_closed();

    if (has_done()) {
      return future_from_done();
    }

    return future_from_new_pending_task();
  }

  bool is_closed() const noexcept override {
    std::lock_guard lock(mutex_);
    return closed_;
  }

  void close() override {
    std::unique_lock lock(mutex_);
    throw_exception_if_closed();

    closed_ = true;

    while (!pending_promises_.empty()) {
      auto promise = pending_promises_.front();
      promise.set_value(package_t{});
    }
  }

  size_t limit() const noexcept override { return LIMIT; }

  channel_t(const channel_t&) = delete;
  channel_t(channel_t&&) = delete;
  channel_t& operator=(const channel_t&) = delete;
  channel_t& operator=(channel_t&&) = delete;

 private:
  void throw_exception_if_closed() {
    if (closed_) {
      throw std::runtime_error("channel is closed");
    }
  }

  void throw_if_channel_overflowed() {
    if (total_ + 1 >= LIMIT) {
      throw std::runtime_error("channel is overflowed");
    }
  }

  bool has_pending() const noexcept { return !pending_promises_.empty(); }

  bool has_done() const noexcept { return !done_futures_.empty(); }

  std::future<package_t> future_from_done() {
    auto future = std::move(done_futures_.front());
    done_futures_.pop();
    return future;
  }

  std::future<package_t> future_from_new_pending_task() {
    std::promise<package_t> promise{};
    auto future = promise.get_future();
    pending_promises_.push(std::move(promise));
    throw_if_channel_overflowed();
    total_++;
    return future;
  }

  void write_in_pending(package_t package) {
    auto promise = std::move(pending_promises_.front());
    pending_promises_.pop();
    promise.set_value(package);
  }

  void write_in_done(package_t package) {
    auto promise = std::promise<package_t>();
    auto future = promise.get_future();
    promise.set_value(package);
    throw_if_channel_overflowed();
    total_++;
    done_futures_.push(std::move(future));
  }

  mutable std::mutex mutex_;

  std::queue<std::future<package_t>> done_futures_;
  std::queue<std::promise<package_t>> pending_promises_;

  bool closed_{};
  size_t total_{};
};

template <class package_t>
class channel_t<package_t, 1> : public channel<package_t> {
 public:
  explicit channel_t()
      : mutex_{}, stream_{std::nullopt}, closed_{}, written_{}, read_{} {}

  ~channel_t() override { close(); }

  void write(package_t value) override {
    std::unique_lock lock(mutex_);

    runtime_check(closed_, "channel is closed");
    runtime_check(written_, "channel is overflowed");

    if (stream_.has_value()) {
      write_into_existed(value);
    } else {
      write_into_new(value);
    }
  }

  std::future<package_t> read() override {
    std::unique_lock lock(mutex_);

    runtime_check(closed_, "channel is closed");
    runtime_check(read_, "channel is already read");

    if (stream_.has_value()) {
      return from_existed();
    } else {
      return from_new();
    }
  }

  bool is_closed() const noexcept override {
    std::unique_lock lock(mutex_);
    return closed_;
  }

  void close() override {
    std::unique_lock lock(mutex_);
    closed_ = true;
    if (stream_.has_value() && !written_) {
      write_into_existed(package_t{});
    }
  }

  size_t limit() const noexcept { return 1; }

  channel_t(const channel_t&) = delete;
  channel_t(channel_t&&) = delete;
  channel_t& operator=(const channel_t&) = delete;
  channel_t& operator=(channel_t&&) = delete;

 private:
  static void runtime_check(bool v, const std::string& msg) {
    if (v) {
      throw std::runtime_error(msg);
    }
  }

  void write_into_existed(package_t value) {
    auto promise = std::move(stream_.value());
    promise.set_value(value);
    stream_ = std::optional(std::move(promise));
    written_ = true;
  }

  void write_into_new(package_t value) {
    std::promise<package_t> promise;
    promise.set_value(value);
    stream_ = std::optional(std::move(promise));
    written_ = true;
  }

  std::future<package_t> from_existed() {
    auto promise = std::move(stream_.value());
    auto future = promise.get_future();
    stream_ = std::optional(std::move(promise));
    read_ = true;
    return future;
  }

  std::future<package_t> from_new() {
    std::promise<package_t> promise;
    auto future = promise.get_future();
    stream_ = std::optional(std::move(promise));
    read_ = true;
    return future;
  }

  mutable std::mutex mutex_;
  std::optional<std::promise<package_t>> stream_;

  bool closed_;
  bool written_;
  bool read_;
};

}  // namespace cnm::communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP