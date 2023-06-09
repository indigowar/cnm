#ifndef HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP
#define HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP

#include <future>
#include <mutex>
#include <shared_mutex>

#include "communication/channels/exceptions.hpp"
#include "communication/channels/storage.hpp"

namespace Cnm::Communication {

template <class T>
class BaseChannel {
 public:
  virtual ~BaseChannel() = default;
  [[nodiscard]] virtual bool isBuffered() const noexcept = 0;
  [[nodiscard]] virtual bool isClosed() const noexcept = 0;
  [[nodiscard]] virtual size_t getLimit() const = 0;
  [[nodiscard]] virtual size_t getAmountOfContained() const noexcept = 0;
};

template <class T>
class ReadableChannel : public BaseChannel<T> {
 public:
  virtual ReadableChannel<T>& operator>>(T& value) = 0;
  virtual ReadableChannel<T>& operator>>(std::future<T>& value) = 0;
};

template <class T>
class WritableChannel : public BaseChannel<T> {
 public:
  virtual WritableChannel<T>& operator<<(T value) = 0;
};

template <class T>
class Channel : public ReadableChannel<T>, public WritableChannel<T> {
 public:
  static Channel<T> makeWithLimit(size_t limit_value) {
    return Channel<T>(channel_storage<T>(limit_value));
  }

  static Channel<T> makeUnbuffered() {
    return Channel<T>(channel_storage<T>());
  }

  explicit Channel(channel_storage<T>&& storage)
      : storage(std::move(storage)) {}

  ~Channel() = default;

  [[nodiscard]] bool isBuffered() const noexcept override {
    std::shared_lock lock(mutex);
    return storage.has_limit();
  }

  [[nodiscard]] bool isClosed() const noexcept override {
    std::shared_lock lock(mutex);
    return storage.is_closed();
  }

  [[nodiscard]] size_t getLimit() const override {
    std::shared_lock lock(mutex);
    if (!storage.has_limit()) {
      throw Exceptions::ChannelUnbuffered();
    }
    return storage.limit();
  }

  [[nodiscard]] size_t getAmountOfContained() const noexcept override {
    std::shared_lock lock(mutex);
    return storage.size();
  }

  void close() {
    std::unique_lock lock(mutex);
    if (storage.is_closed()) {
      throw Exceptions::ChannelIsClosed();
    }
    storage.close();
  }

  // write method
  WritableChannel<T>& operator<<(T value) override {
    std::unique_lock lock(mutex);
    storage.push(value);
    return *this;
  }

  // sync read
  ReadableChannel<T>& operator>>(T& value) override {
    auto future = readFromStorage();
    future.wait();
    value = future.get();
    return *this;
  }

  // async read
  ReadableChannel<T>& operator>>(std::future<T>& value) override {
    value = std::move(readFromStorage());
    return *this;
  }

  Channel(const Channel<T>&) = delete;
  Channel& operator=(const Channel<T>&) = delete;

  Channel(Channel<T>&&) = delete;
  Channel& operator=(Channel<T>&&) = delete;

 private:
  std::future<T> readFromStorage() {
    std::unique_lock lock(mutex);
    return storage.pop();
  }

  channel_storage<T> storage;

  mutable std::shared_mutex mutex;
};

}  // namespace Cnm::Communication

#endif  // HPP_CNM_LIB_COMMUNICATION_CHANNELS_CHANNEL_HPP
