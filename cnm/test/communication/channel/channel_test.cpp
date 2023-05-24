#include "communication/channels/channel.hpp"

#include <gtest/gtest.h>

#include <array>
#include <numeric>
#include <ranges>

using namespace Cnm::Communication;

TEST(ChannelTest, ChannelCreationWith_makeWithLimit) {
  auto chan = Channel<int>::makeWithLimit(5);
  EXPECT_TRUE(chan.isBuffered());
  EXPECT_EQ(chan.getLimit(), 5);
  EXPECT_EQ(chan.getAmountOfContained(), 0);
  EXPECT_FALSE(chan.isClosed());
}

TEST(ChannelTest, ChannelCreationWith_makeUnbuffered) {
  auto chan = Channel<int>::makeUnbuffered();
  EXPECT_FALSE(chan.isBuffered());
  EXPECT_THROW(chan.getLimit(), Exceptions::ChannelUnbuffered);
  EXPECT_EQ(chan.getAmountOfContained(), 0);
  EXPECT_FALSE(chan.isClosed());
}

TEST(ChannelTest, WrittingInChannel) {
  auto chan = Channel<int>::makeUnbuffered();

  std::array<int, 10> array{};
  std::iota(array.begin(), array.end(), 0);

  std::for_each(array.begin(), array.end(), [&chan](int i) { chan << i; });

  EXPECT_EQ(chan.getAmountOfContained(), 10);
}

TEST(ChannelTest, ReadingFromChannel) {
  auto chan = Channel<int>::makeUnbuffered();

  chan << 50 << 100;

  int first{}, second{};
  chan >> first >> second;

  EXPECT_EQ(first, 50);
  EXPECT_EQ(second, 100);
}

TEST(ChannelTest, ClosingChannel) {
  auto chan = Channel<int>::makeUnbuffered();

  EXPECT_FALSE(chan.isClosed());

  chan.close();

  EXPECT_THROW(
      [&chan] {
        int i{};
        chan >> i;
      }(),
      Exceptions::ChannelIsClosed);
  EXPECT_TRUE(chan.isClosed());
}

TEST(ChannelTest, ReadingFromClosedChannel) {
  // reading from closing Channel should be available

  auto chan = Channel<int>::makeUnbuffered();

  chan << 600 << 1000 << 3000;

  chan.close();

  EXPECT_TRUE(chan.isClosed());

  EXPECT_THROW(
      [&chan] {
        int i{};
        chan >> i;
      }(),
      Exceptions::ChannelIsClosed);
}
