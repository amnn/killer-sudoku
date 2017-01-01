#include <gtest/gtest.h>

#include <disjoint_set/DisjointSet.h>

using namespace disjoint_set;

namespace {
  struct Adder {
    inline uint8_t operator()(uint8_t &x, uint8_t &y) const
    {
      return x + y;
    }
  };
}

TEST(DisjointSetTest, Dereference)
{
  Set<uint8_t> s(42);
  EXPECT_EQ(42, *s);
}

TEST(DisjointSetTest, FindSelf)
{
  Set<uint8_t> s(42);
  EXPECT_EQ(&s, s.find());
}

TEST(DisjointSetTest, Union)
{
  Set<uint8_t> s(1), t(2);
  s += t;

  EXPECT_EQ(s.find(), t.find());
}

TEST(DisjointSetTest, UnionSelf)
{
  Set<uint8_t> s(1);
  auto p = s.find();
  s += s;

  EXPECT_EQ(p, s.find());
}

TEST(DisjointSetTest, Merging) {
  Set<uint8_t, Adder> s(18), t(24);
  s += t;

  EXPECT_EQ(42, **s.find());
}
