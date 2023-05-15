#include "communication/channels/storage.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "communication/channels/exceptions.hpp"

using namespace cnm::communication;

TEST(channel_storage, normal_operation) {
  // Test with a limit of 3
  channel_storage<int> cs(3);
  ASSERT_EQ(cs.size(), 0);

  // Push 3 values, no exceptions should be thrown
  cs.push(1);
  ASSERT_EQ(cs.size(), 1);
  cs.push(2);
  ASSERT_EQ(cs.size(), 2);
  cs.push(3);
  ASSERT_EQ(cs.size(), 3);

  // The fourth value should throw an exception
  EXPECT_THROW(cs.push(4), exceptions::channel_overflowed_error);
  ASSERT_EQ(cs.size(), 3);

  // Pop values from the storage
  auto f1 = cs.pop();
  auto f2 = cs.pop();
  auto f3 = cs.pop();
}

TEST(ChannelStorage, FutureShouldWaitForValue) {
  // Test with a limit of 1
  channel_storage<int> cs(1);
  ASSERT_EQ(cs.size(), 0);

  // Get a future before pushing any values
  auto f1 = cs.pop();
  ASSERT_TRUE(f1.valid());

  // The future should wait for values to appear
  std::thread t([&cs]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    cs.push(1);
  });
  auto start = std::chrono::steady_clock::now();
  auto result = f1.get();
  auto end = std::chrono::steady_clock::now();

  EXPECT_EQ(result, 1);
  ASSERT_TRUE(end - start >= std::chrono::milliseconds(500));
  t.join();
}

TEST(ChannelStorage, UnbufferedChannel) {
  // Test with an unbuffered channel (limit = 0)
  channel_storage<int> cs;

  // Push values, no exceptions should be thrown and all of them should be
  // immediately available
  cs.push(1);
  ASSERT_EQ(cs.size(), 1);
  cs.push(2);
  ASSERT_EQ(cs.size(), 2);
  cs.push(3);
  ASSERT_EQ(cs.size(), 3);

  // Pop values from the storage, they should all be immediately available
  auto f1 = cs.pop();
  ASSERT_TRUE(f1.valid());
  EXPECT_EQ(f1.get(), 1);
  auto f2 = cs.pop();
  ASSERT_TRUE(f2.valid());
  EXPECT_EQ(f2.get(), 2);
  auto f3 = cs.pop();
  ASSERT_TRUE(f3.valid());
  EXPECT_EQ(f3.get(), 3);
}