#include <gtest/gtest.h>

#include <utils/thread_safe_queue.hpp>

TEST(thread_safe_queue, default_behavior) {
  cnm::utils::thread_safe_queue<int> q;

  EXPECT_EQ(q.size(), 0);
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.pop(), std::nullopt);
}

TEST(thread_safe_queue, push) {
  cnm::utils::thread_safe_queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);
  q.push(5);

  EXPECT_EQ(q.size(), 5);
  EXPECT_FALSE(q.empty());

  for (int i{1}; i < 6; i++) {
    auto from_queue = q.pop();
    EXPECT_NE(from_queue, std::nullopt);
    EXPECT_EQ(from_queue.value(), i);
  }
}
