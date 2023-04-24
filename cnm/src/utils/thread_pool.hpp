#ifndef HPP_CNM_LIB_UTILS_THREAD_POOL_HPP
#define HPP_CNM_LIB_UTILS_THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <chrono>

namespace cnm::utils {

// TODO: write
//  https://github.com/bshoshany/thread-pool/blob/master/BS_thread_pool_light.hpp#L210
class thread_pool final {
 public:
  explicit thread_pool(size_t);

  ~thread_pool();

  template <class Func, class... Args>
  void push(Func&& function, Args&&... args) {
    std::function<void()> task =
        std::bind(std::forward<Func>(function), std::forward<Args>(args)...);
    {
      std::scoped_lock lock(m_mutex);
      m_tasks.push(task);
    }
    ++m_total;
    m_available.notify_one();
  }

  template <class Func, class... Args,
            class Result =
                std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>>
  std::future<Result> submit(Func&& func, Args&&... args) {
    std::function<Result()> task =
        std::find(std::forward<Func>(func), std::forward<Args>(args)...);
    std::shared_ptr<std::promise<Result>> promise =
        std::make_shared<std::promise<Result>>();

    push([task, promise] {
      try {
        if constexpr (std::is_void_v<Result>) {
          std::invoke(task);
          promise->set_value();
        } else {
          promise->set_value(std::invoke(task));
        }
      } catch (...) {
        try {
          promise->set_exception(std::current_exception());
        } catch (...) {
        }
      }
    });
    return promise->get_future();
  }

  void graceful_shutdown(const std::chrono::duration& wait_time);

  void terminate();

 private:
  std::atomic_bool m_running;
  std::condition_variable m_available;
  std::condition_variable m_done;
  std::queue<std::function<void()>> m_tasks;
  std::atomic<size_t> m_total;
  mutable std::mutex m_mutex;
  std::atomic_bool m_waiting;
  std::atomic_bool m_blocked;

  std::vector<std::thread> m_threads;
};

}  // namespace cnm::utils

#endif  // HPP_CNM_LIB_UTILS_THREAD_POOL_HPP