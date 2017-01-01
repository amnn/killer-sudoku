#include <gtest/gtest.h>

#include <stdexcept>

#include <region/Region.h>
using namespace region;

TEST(RegionTest, TwoConstraints) {
  Region r(1, 1, 2), s(2, 3, 4);

  RegionCombiner rc;
  EXPECT_THROW(rc(r, s), std::invalid_argument);
}

TEST(RegionTest, NoConstraints) {
  Region r(1, 2), s(3, 4);

  RegionCombiner rc;
  EXPECT_NO_THROW({
      auto t = rc(r, s);
      EXPECT_FALSE(t.isConstrained);
    });
}

TEST(RegionTest, LeftConstraint) {
  Region r(1, 1, 2), s(3, 4);

  RegionCombiner rc;
  EXPECT_NO_THROW({
      auto t = rc(r, s);
      EXPECT_EQ(r, t);
    });
}

TEST(RegionTest, RightConstraint) {
  Region r(1, 2), s(2, 3, 4);

  RegionCombiner rc;
  EXPECT_NO_THROW({
      auto t = rc(r, s);
      EXPECT_EQ(s, t);
    });
}
