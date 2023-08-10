#ifndef HPP_CNM_CORE_UTILS_THREAD_POOL_HPP
#define HPP_CNM_CORE_UTILS_THREAD_POOL_HPP

#include <functional>
#include <future>
#include <queue>
#include <thread>

namespace Utils {

class ThreadPool {
 public:
  explicit ThreadPool(size_t numWorkers)
      : stop(false), workers_count{numWorkers} {}

  ~ThreadPool() { stopThreads(); }

  template <typename F, typename... Args>
  auto enqueue(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
    using ReturnType = decltype(f(args...));
    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    auto future = task->get_future();
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      taskQueue.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return future;
  }

  void startThreads() {
    workers.reserve(workers_count);
    for (size_t i{}; i != workers_count; i++) {
      workers.emplace_back(
          [this](const std::stop_token &token) { workerThread(token); });
    }
  }

  void stopThreads() {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      stop = true;
    }
    condition.notify_all();
    for (auto &worker : workers) {
      if (worker.joinable()) {
        worker.join();
      }
    }
    workers.clear();
  }

  [[nodiscard]] size_t getNumBusyWorkers() const { return workers.size(); }

  [[nodiscard]] size_t getNumFreeWorkers() const {
    return workers.size() - taskQueue.size();
  }

  [[nodiscard]] size_t getTaskQueueSize() const { return taskQueue.size(); }

 private:
  std::vector<std::jthread> workers;
  std::queue<std::function<void()>> taskQueue;
  std::mutex queueMutex;
  std::condition_variable condition;
  bool stop;
  size_t workers_count;

  void workerThread(const std::stop_token &token) {
    while (!token.stop_requested()) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [this] { return stop || !taskQueue.empty(); });
        if (stop && taskQueue.empty()) {
          return;
        }
        task = std::move(taskQueue.front());
        taskQueue.pop();
      }
      task();
    }
  }
};

}  // namespace Utils

#endif  // HPP_CNM_CORE_UTILS_THREAD_POOL_HPP
