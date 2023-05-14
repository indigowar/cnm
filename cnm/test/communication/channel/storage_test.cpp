#include <gtest/gtest.h>

#include <communication/channels/storage.hpp>

using namespace cnm::communication;

TEST(channel_internal_storage_test, main_test) {
  channel_storage<int> storage{};

  storage.push(5);
  storage.push(1);

  EXPECT_EQ(storage.size(), 2);

  auto first = storage.pop().get();
  auto second = storage.pop().get();

  EXPECT_EQ(first, 5);
  EXPECT_EQ(second, 1);

  auto third = storage.pop();

  EXPECT_EQ(storage.size(), 1);

  storage.push(666);

  EXPECT_EQ(third.get(), 666);
}