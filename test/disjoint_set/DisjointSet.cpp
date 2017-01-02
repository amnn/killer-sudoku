#include <gtest/gtest.h>

#include <disjoint_set/Arena.h>

using namespace disjoint_set;

namespace {
  struct Adder {
    inline uint8_t operator()(const uint8_t &x, const uint8_t &y) const
    {
      return x + y;
    }
  };
}

TEST(DisjointSetTest, Dereference)
{
  Arena<uint8_t> a;
  auto &s = *a.newSet(42);
  EXPECT_EQ(42, *s);
}

TEST(DisjointSetTest, FindSelf)
{
  Arena<uint8_t> a;
  auto &s = *a.newSet(42);
  EXPECT_EQ(&s, s.find());
}

TEST(DisjointSetTest, Union)
{
  Arena<uint8_t> a;
  auto &s = *a.newSet(1);
  auto &t = *a.newSet(2);
  s += t;

  EXPECT_EQ(s.find(), t.find());
}

TEST(DisjointSetTest, UnionSelf)
{
  Arena<uint8_t> a;
  auto &s = *a.newSet(1);
  auto p = s.find();
  s += s;

  EXPECT_EQ(p, s.find());
}

TEST(DisjointSetTest, Merging) {
  Arena<uint8_t, Adder> a;
  auto &s = *a.newSet(18);
  auto &t = *a.newSet(24);
  s += t;

  auto &p = *s.find();

  EXPECT_EQ(42, *p);
}