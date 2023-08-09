#ifndef HPP_CNM_CORE_UTILS_THREAD_POOL_HPP
#define HPP_CNM_CORE_UTILS_THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include "cnm/utils/thread_safe_queue.hpp"

namespace Utils {

class ThreadPool final {
  using FunctionType = std::function<void()>;

  struct TaskItem {
    ThreadSafeQueue<FunctionType> tasks;
    std::binary_semaphore signal{0};
  };

 public:
  explicit ThreadPool(size_t amountOfWorkers) : tasks(amountOfWorkers) {
    size_t current{};
    for (size_t i{}; i < amountOfWorkers; i++) {
      priority_queue.pushBack(size_t(current));
      try {
        threads.emplace_back([this, &current](const std::stop_token& token) {
          threadRunner(token, current);
        });
        current++;
      } catch (...) {
        tasks.pop_back();
      }
    }
  }

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  ~ThreadPool() {
    for (size_t i{}; i < threads.size(); i++) {
      threads[i].request_stop();
      tasks[i].signal.release();
      threads[i].join();
    }
  }

  size_t size() const noexcept { return threads.size(); }

  template <typename Function, typename... Args>
  auto add(Function f, Args... args) -> std::future<decltype(f(args...))> {
    using ReturnType = decltype(f(args...));

    auto promise = std::make_shared<std::promise<ReturnType>>();
    auto task = [func = std::move(f), ... args = std::move(args),
                 promise = promise] {
      try {
        if constexpr (std::is_same_v<ReturnType, void>) {
          func(args...);
          promise->set_value();
        } else {
          promise->set_value(func(args...));
        }
      } catch (...) {
        promise->set_exception(std::current_exception());
      }
    };

    auto future = promise->get_future();
    addTask(std::move(task));

    return future;
  }

  template <typename Function, typename... Args>
    requires std::invocable<Function, Args...> &&
             std::is_same_v<void, std::invoke_result_t<Function&&, Args&&...>>
  void addDetach(Function function, Args... args) {
    this->add(std::move([f = std::forward<Function>(function),
                         ... a = std::forward<Args>(args)] {
      try {
        std::invoke(f, a...);
      } catch (...) {
      }
    }));
  }

 private:
  template <typename Function>
  void addTask(Function&& f) {
    auto i_opt = priority_queue.copyFrontAndRotateToBack();
    if (!i_opt.has_value()) {
      return;
    }

    auto i = *i_opt;
    pending_tasks.fetch_add(1, std::memory_order_relaxed);
    tasks[i].tasks.pushBack(std::forward<Function>(f));
    tasks[i].signal.release();
  }

  void threadRunner(const std::stop_token& stop_token, size_t id) {
    do {
      tasks[id].signal.acquire();

      do {
        while (auto task = tasks[id].tasks.popFront()) {
          try {
            pending_tasks.fetch_sub(1, std::memory_order_release);
            std::invoke(std::move(task.value()));
          } catch (...) {
          }
        }

        for (size_t i = 1; i < tasks.size(); ++i) {
          const size_t index = (id + i) % tasks.size();
          if (auto task = tasks[index].tasks.steal()) {
            pending_tasks.fetch_sub(1, std::memory_order_release);
            std::invoke(std::move(task.value()));
            break;
          }
        }

      } while (pending_tasks.load(std::memory_order_acquire) > 0);

      priority_queue.rotateToFront(id);

    } while (stop_token.stop_requested());
  }

  std::vector<std::jthread> threads;
  std::deque<TaskItem> tasks;
  ThreadSafeQueue<size_t> priority_queue;
  std::atomic_int_fast64_t pending_tasks{};
};

}  // namespace Utils

#endif  // HPP_CNM_CORE_UTILS_THREAD_POOL_HPP
