#include <gtest/gtest.h>

#include <vector>

#include <reversed/Reversed.h>
using namespace reversed;

TEST(ReversedTest, Empty) {
  std::vector<uint8_t> empty {};

  std::vector<uint8_t> actual {};

  for (auto x : reverse(empty)) {
    actual.push_back(x);
  }

  EXPECT_EQ(empty, actual);
}

TEST(ReversedTest, NonEmpty) {
  std::vector<uint8_t> orig   {1, 2, 3, 4};
  std::vector<uint8_t> expect {4, 3, 2, 1};
  std::vector<uint8_t> actual {};

  for (auto x : reverse(orig)) {
    actual.push_back(x);
  }

  EXPECT_EQ(expect, actual);
}


TEST(ReversedTest, SelfInverse) {
  std::vector<uint8_t> orig   {1, 2, 3, 4};
  std::vector<uint8_t> actual {};

  for (auto x : reverse(reverse(orig))) {
    actual.push_back(x);
  }

  EXPECT_EQ(orig, actual);
}
