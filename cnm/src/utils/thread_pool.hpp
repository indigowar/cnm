#ifndef HPP_CNM_LIB_UTILS_THREAD_POOL_HPP
#define HPP_CNM_LIB_UTILS_THREAD_POOL_HPP

#include <any>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

namespace Cnm::Utils {

class ThreadPool final {
 public:
  ThreadPool(size_t workers)
      : workers(workers), tasks(), runningTasks{}, stop{}, shutdown{} {
    for (auto& w : this->workers) {
      w = std::thread([this] { worker_func(); });
    }
  }

  ~ThreadPool() { kill(); }

  template <typename Func, typename... Args>
  auto push(Func&& f, Args&&... args) -> std::future<decltype(func(args...))> {
    if (shutdown) {
      throw std::runtime_error("ThreadPool is dead");
    }
    using result_t = decltype(func(args...));

    auto task = std::make_shared<std::packaged_task<result_t()>>(
        std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

    auto result = task->get_future();

    std::unique_lock lock(mutex);
    tasks.emplace([task] { (*task)(); });
    thread_notifier.notify_one();

    return result;
  }

  void freeze() {
    std::unique_lock lock(mutex);
    stop = true;
  }

  void invoke() {
    std::unique_lock lock(mutex);
    if (stop && shutdown) {
      throw std::runtime_error("ThreadPool is dead");
    }
    if (!stop) {
      throw std::runtime_error("ThreadPool is already active");
    }
    stop = false;
  }

  void kill() {
    std::unique_lock lock(mutex);
    stop = shutdown = true;
  }

 private:
  void worker_func() {
    for (;;) {
      std::function<void()> task;
      {
        std::unique_lock lock(mutex);
        thread_notifier.wait(lock, [this] { return stop || !tasks.empty(); });
        if (stop && tasks.empty()) {
          return;
        }
        task = std::move(tasks.front());
        tasks.pop();
        runningTasks--;
      }
      task();
      runningTasks--;
      if (shutdown) {
        return;
      }
    }
  }

  std::vector<std::thread> workers;

  std::queue<std::function<void()>> tasks;
  std::mutex mutex;

  std::condition_variable thread_notifier;

  std::atomic<int> runningTasks;

  bool stop;
  bool shutdown;
};

}  // namespace Cnm::Utils
#endif  // HPP_CNM_LIB_UTILS_THREAD_POOL_HPP
