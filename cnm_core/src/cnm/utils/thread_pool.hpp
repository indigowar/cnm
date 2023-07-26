#ifndef HPP_CNM_CORE_UTILS_THREAD_POOL_HPP
#define HPP_CNM_CORE_UTILS_THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace Utils {

class ThreadPool final {
 public:
  ThreadPool(size_t workers_amount) {
    for (size_t i{}; i < workers_amount; i++) {
      workers.emplace_back(std::bind(&ThreadPool::worker, this));
    }
  }

  ~ThreadPool() {
    // TODO: call stop()
    stop();
  }

  template <typename F, typename... Args>
  auto execute(F&& func, Args&&... args)
      -> std::future<decltype(func(args...))> {
    using ReturnType = decltype(func(args...));

    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<F>(func), std::forward<Args>(args)...));

    auto future = task->get_future();

    {
      std::unique_lock lock(mutex);
      tasks.push([task] { task(); });
    }

    return future;
  }

  void stop() {
    {
      std::unique_lock lock(mutex);
      is_stopped = true;
    }
    condition.notify_all();
    for (auto& i : workers) {
      i.request_stop();
    }
  }

 private:
  void worker() {}

  std::shared_mutex mutex;
  std::vector<std::jthread> workers;
  std::queue<std::function<void()>> tasks;
  std::condition_variable condition;

  bool is_stopped;
};

}  // namespace Utils

#endif  // HPP_CNM_CORE_UTILS_THREAD_POOL_HPP
