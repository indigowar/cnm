#include "communication/channel.hpp"

#include <gtest/gtest.h>

using namespace cnm::communication;

TEST(ChannelTest, ConstructorCreatesOnceChannelRight) {
  auto once = channel_t<int, 1>();

  EXPECT_EQ(once.limit(), 1);
  EXPECT_FALSE(once.is_closed());
}

TEST(ChannelTest, ConstructorCreatesMultipleChannelRight) {
  auto multiple_channel = channel_t<int, 50>();

  EXPECT_EQ(multiple_channel.limit(), 50);
  EXPECT_FALSE(multiple_channel.is_closed());
}

TEST(ChannelTest, WritingAndReadingInOnceChannel) {
  channel_t<int, 1> chan{};

  const int data = 500;

  chan.write(data);

  auto result = chan.read().get();

  EXPECT_EQ(data, result);
}
