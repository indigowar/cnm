#include "communication/channels/channel.hpp"

#include <gtest/gtest.h>

#include <array>
#include <numeric>
#include <ranges>

using namespace cnm::communication;

TEST(channel_test, channel_creation_with_make_with_limit) {
  auto chan = channel<int>::make_with_limit(5);
  EXPECT_TRUE(chan.is_buffered());
  EXPECT_EQ(chan.get_limit(), 5);
  EXPECT_EQ(chan.get_amount_of_contained(), 0);
  EXPECT_FALSE(chan.is_closed());
}

TEST(channel_test, channel_creation_with_make_unbuffered) {
  auto chan = channel<int>::make_unbuffered();
  EXPECT_FALSE(chan.is_buffered());
  EXPECT_THROW(chan.get_limit(), exceptions::channel_unbuffered_error);
  EXPECT_EQ(chan.get_amount_of_contained(), 0);
  EXPECT_FALSE(chan.is_closed());
}

TEST(channel_test, writting_in_channel) {
  auto chan = channel<int>::make_unbuffered();

  std::array<int, 10> array{};
  std::iota(array.begin(), array.end(), 0);

  std::for_each(array.begin(), array.end(), [&chan](int i) { chan << i; });

  EXPECT_EQ(chan.get_amount_of_contained(), 10);
}

TEST(channel_test, reading_from_channel) {
  auto chan = channel<int>::make_unbuffered();

  chan << 50 << 100;

  int first{}, second{};
  chan >> first >> second;

  EXPECT_EQ(first, 50);
  EXPECT_EQ(second, 100);
}

TEST(channel_test, closing_channel) {
  auto chan = channel<int>::make_unbuffered();

  EXPECT_FALSE(chan.is_closed());

  chan.close();

  EXPECT_THROW(
      [&chan] {
        int i{};
        chan >> i;
      }(),
      exceptions::channel_closed_error);
  EXPECT_TRUE(chan.is_closed());
}

TEST(channel_test, reading_from_closing_channel) {
  // reading from closing channel should be available

  auto chan = channel<int>::make_unbuffered();

  chan << 600 << 1000 << 3000;

  chan.close();

  EXPECT_TRUE(chan.is_closed());

  EXPECT_THROW(
      [&chan] {
        int i{};
        chan >> i;
      }(),
      exceptions::channel_closed_error);

  int first{}, second{}, third{};

  auto task = [&first, &second, &third, &chan] {
    chan >> first >> second >> third;
  };

  EXPECT_NO_THROW(task());

  EXPECT_EQ(first, 600);
  EXPECT_EQ(second, 1000);
  EXPECT_EQ(third, 3000);
}
