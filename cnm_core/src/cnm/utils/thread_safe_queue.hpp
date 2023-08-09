#ifndef HPP_CNM_CORE_UTILS_THREAD_SAFE_QUEUE_HPP
#define HPP_CNM_CORE_UTILS_THREAD_SAFE_QUEUE_HPP

#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <optional>
#include <thread>
#include <type_traits>

namespace Utils {

template <typename T>
concept is_lockable = requires(T&& t) {
  t.lock();
  t.unlock();
  { t.try_lock() } -> std::convertible_to<bool>;
};

template <typename T, typename Lock = std::mutex>
  requires is_lockable<Lock>
class ThreadSafeQueue {
 public:
  ThreadSafeQueue() = default;

  void pushBack(T&& t) {
    std::unique_lock lock(sync);
    data.push_back(t);
  }

  void pushFront(T&& t) {
    std::unique_lock lock(sync);
    data.push_front(t);
  }

  bool isEmpty() const noexcept {
    std::unique_lock lock(sync);
    return data.empty();
  }

  std::optional<T> steal() {
    std::unique_lock lock(sync);
    if (data.empty()) return std::nullopt;

    auto back = std::move(data.back());
    data.pop_back();
    return back;
  }

  void rotateToFront(const T& item) {
    std::unique_lock lock(sync);

    auto iterator = std::find(data.begin(), data.end(), item);
    if (iterator != data.end()) {
      data.erase(iterator);
    }

    data.push_back(item);
  }

  std::optional<T> copyFrontAndRotateToBack() {
    std::unique_lock lock(sync);

    if (data.empty()) return std::nullopt;

    auto front = data.front();
    data.pop_front();
    data.push_back(front);

    return front;
  }

  std::optional<T> popBack() {
    std::unique_lock lock(sync);
    if (data.empty()) return std::nullopt;

    auto back = std::move(data.back());
    data.pop_back();

    return back;
  }

  std::optional<T> popFront() {
    std::unique_lock lock(sync);
    if (data.empty()) return std::nullopt;

    auto front = std::move(data.front());

    return front;
  }

 private:
  std::deque<T> data;

  mutable Lock sync;
};

}  // namespace Utils

#endif  // HPP_CNM_CORE_UTILS_THREAD_SAFE_QUEUE_HPP
