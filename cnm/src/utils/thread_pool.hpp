#ifndef HPP_CNM_LIB_UTILS_THREAD_POOL_HPP
#define HPP_CNM_LIB_UTILS_THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace Cnm::utils {

class thread_pool {
  using task_type = std::function<void()>;

 public:
  explicit thread_pool(size_t num) {
    for (size_t i{}; i < num; i++) {
      workers_.emplace_back([this] {
        while (true) {
          task_type task;
          {
            std::unique_lock<std::mutex> lock(task_mutex_);
            task_cond_.wait(lock, [this] { return !tasks_.empty(); });
            task = std::move(tasks_.front());
            tasks_.pop();
          }
          if (!task) {
            push_stop_task();
            return;
          }
          task();
        }
      });
    }
  }

  ~thread_pool() { stop(); }

  void stop() {
    push_stop_task();
    for (auto& worker : workers_) {
      if (worker.joinable()) {
        worker.join();
      }
    }
    std::queue<task_type> empty{};
    std::swap(tasks_, empty);
  }

  template <typename F, typename... Args>
  auto push(F&& f, Args&&... args) {
    using return_type = std::invoke_result_t<F, Args...>;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    auto res = task->get_future();

    {
      std::lock_guard<std::mutex> lock(task_mutex_);
      tasks_.emplace([task] { (*task)(); });
    }
    task_cond_.notify_one();

    return res;
  }

 private:
  void push_stop_task() {
    std::lock_guard<std::mutex> lock(task_mutex_);
    tasks_.emplace();
    task_cond_.notify_one();
  }

  std::vector<std::thread> workers_{};
  std::queue<task_type> tasks_{};
  std::mutex task_mutex_;
  std::condition_variable task_cond_;
};

}  // namespace Cnm::utils
#endif  // HPP_CNM_LIB_UTILS_THREAD_POOL_HPP
