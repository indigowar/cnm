#include "cnm/utils/thread_safe_queue.hpp"

#include <gtest/gtest.h>

#include <barrier>
#include <future>
#include <thread>

using namespace Utils;

TEST(ThreadSafeQueueTest, InsertionAndPopups) {
  std::barrier barrier(3);
  std::promise<int> p1, p2, p3;

  {
    ThreadSafeQueue<int> queue;

    std::jthread t1([&queue, &barrier, &p1] {
      barrier.arrive_and_wait();
      queue.pushFront(1);
      barrier.arrive_and_wait();
      p1.set_value(queue.popBack().value_or(-1));
    });
    std::jthread t2([&queue, &barrier, &p2] {
      barrier.arrive_and_wait();
      queue.pushFront(2);
      barrier.arrive_and_wait();
      p2.set_value(queue.popBack().value_or(-1));
    });
    std::jthread t3([&queue, &barrier, &p3] {
      barrier.arrive_and_wait();
      queue.pushFront(3);
      barrier.arrive_and_wait();
      p3.set_value(queue.popBack().value_or(-1));
    });
  }
}
