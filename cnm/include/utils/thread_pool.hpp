#ifndef HPP_CNM_LIB_UTILS_THREAD_POOL_HPP
#define HPP_CNM_LIB_UTILS_THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace cnm::utils {

class thread_pool {
 public:
  thread_pool() : m_stop_flag(false) {
    for (size_t it = 0; it < m_threads_amount; it++) {
      threads.push_back(std::thread(&thread_pool::thread_loop, this));
    }
  }

  ~thread_pool() {
    for (size_t it = 0; it < m_threads_amount; it++) {
      threads.at(it).join();
    }
  }

  void thread_loop() noexcept {
    std::function<void()> func;

    while (true) {
      {
        std::unique_lock<std::mutex> lock(m_lock);
        m_condition.wait(lock,
                         [this]() { return !m_tasks.empty() || m_stop_flag; });
        if (m_stop_flag && m_tasks.empty()) {
          return;
        }
        func = m_tasks.front();
        m_tasks.pop();
      }
      func();
    }
  }

  void push(std::function<void()> func) noexcept {
    std::unique_lock<std::mutex> lock(m_lock);
    m_tasks.push(func);

    lock.unlock();
    m_condition.notify_one();
  }

  void terminate() noexcept {
    std::unique_lock<std::mutex> lock(m_lock);
    m_stop_flag = true;
    lock.unlock();
    m_condition.notify_all();
  }

 private:
  std::vector<std::thread> threads;
  cnm::utils::thread_safe_queue<std::function<void()>> m_tasks;
  //std::queue<std::function<void()>> m_tasks;
  mutable std::mutex m_lock;
  std::atomic<bool> m_stop_flag;
  std::condition_variable m_condition;

  const size_t m_threads_amount = std::thread::hardware_concurrency();
};
}  // namespace cnm::utils

#endif  // HPP_CNM_LIB_UTILS_THREAD_POOL_HPP