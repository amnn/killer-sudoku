#include <gtest/gtest.h>

#include <disjoint_set/DisjointSet.h>

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
  Set<uint8_t>::Arena a;
  auto &s = *a.alloc(42);
  EXPECT_EQ(42, *s);
}

TEST(DisjointSetTest, FindSelf)
{
  Set<uint8_t>::Arena a;
  auto &s = *a.alloc(42);
  EXPECT_EQ(&s, s.find());
}

TEST(DisjointSetTest, Union)
{
  Set<uint8_t>::Arena a;
  auto &s = *a.alloc(1);
  auto &t = *a.alloc(2);
  s += t;

  EXPECT_EQ(s.find(), t.find());
}

TEST(DisjointSetTest, UnionSelf)
{
  Set<uint8_t>::Arena a;
  auto &s = *a.alloc(1);
  auto p = s.find();
  s += s;

  EXPECT_EQ(p, s.find());
}

TEST(DisjointSetTest, Merging) {
  Set<uint8_t, Adder>::Arena a;
  auto &s = *a.alloc(18);
  auto &t = *a.alloc(24);
  s += t;

  auto &p = *s.find();

  EXPECT_EQ(42, *p);
}
