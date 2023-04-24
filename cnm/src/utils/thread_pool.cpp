#include "thread_pool.hpp"

#include <chrono>

using namespace cnm::utils;

class worker final {
 public:
  worker(const std::atomic_bool& running, std::condition_variable& available,
         std::condition_variable& done, std::mutex& mutex,
         std::queue<std::function<void()>>& tasks, std::atomic<size_t>& total)
      : m_running{running},
        m_available{available},
        m_done{done},
        m_mutex{mutex},
        m_tasks{tasks},
        m_total{total} {}

  void operator()() {
    while (m_running) {
      std::unique_lock lock(m_mutex);
      m_available.wait(lock, [this] { return !m_tasks.empty() || !m_running; });
      if (m_running) {
        auto task = std::move(m_tasks.front());
        m_tasks.pop();
        lock.unlock();
        task();
        lock.lock();
        --m_total;
        if (m_running) m_done.notify_one();
      }
    }
  }

 private:
  const std::atomic_bool& m_running;
  std::condition_variable& m_available;
  std::condition_variable& m_done;

  std::mutex& m_mutex;
  std::queue<std::function<void()>>& m_tasks;

  std::atomic<size_t>& m_total;
};

thread_pool::thread_pool(size_t workers_amount)
    : m_running{true},
      m_available{},
      m_done{},
      m_tasks(),
      m_total{},
      m_mutex{},
      m_waiting{},
      m_threads(workers_amount) {
  for (auto& thread : m_threads) {
    thread = std::thread(
        worker(m_running, m_available, m_done, m_mutex, m_tasks, m_total));
  }
}

thread_pool::~thread_pool() {}

void thread_pool::graceful_shutdown(const std::chrono::duration& wait_time) {

}

void thread_pool::terminate() {}
