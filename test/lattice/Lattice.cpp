#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <lattice/Lattice.h>
using namespace lattice;

#include <vector>

using ::testing::ContainerEq;
using ::testing::ElementsAre;
using ::testing::UnorderedElementsAre;

namespace {

  std::vector< std::vector<uint32_t> > ivals(Node *root)
  {

    std::vector< std::vector<uint32_t> >
      vals {};

    for (Node &row : root->vertRange()) {
      vals.emplace_back();
      auto &rowVals = vals.back();

      for (Node &node : row.horizRange()) {
        rowVals.emplace_back(node.val());
      }
    }

    return vals;
  }

} // anonymous namespace

TEST(LatticeTest, Init) {
  Lattice<uint8_t, uint8_t> l(
    {0, 1, 2, 3, 4},
    {0, 1, 2, 3, 4},
    [](uint8_t) { return 0; },
    [](uint8_t r, uint8_t c) {
      return (r + c) % 2
        ? (r + c)
        : 0;
    });

  std::vector< std::vector<uint32_t> >
    expected {{1, 3}, {1, 3, 5}, {3, 5}, {3, 5, 7}, {5, 7}};

  EXPECT_THAT(ivals(l.root()), ContainerEq(expected));
}

/**     1  1  1  1
 *    +------------+
 *    +------------+
 */
TEST(LatticeTest, UnsolvableEmpty) {
  Lattice<uint8_t, uint8_t> l(
    {},
    {0, 1, 2, 3},
    [](uint8_t) { return 1; },
    [](uint8_t, uint8_t) { return 0; });

  EXPECT_EQ(nullptr, l.solve());
}


/**     1  1  1  1
 *    +------------+
 *    | 0  0  0  0 |
 *    +------------+
 */
TEST(LatticeTest, UnsolvableZero) {
  Lattice<uint8_t, uint8_t> l(
    {0},
    {0, 1, 2, 3},
    [](uint8_t) { return 1; },
    [](uint8_t, uint8_t) { return 0; });

  EXPECT_EQ(nullptr, l.solve());
}

/**     1
 *    +---+
 *    | 1 |
 *    +---+
 */
TEST(LatticeTest, SolvableTrivial) {
  Lattice<uint8_t, uint8_t> l(
    {0},
    {0},
    [](uint8_t) { return 1; },
    [](uint8_t, uint8_t) { return 1; });

  auto dd = l.debugDump();
  auto soln = l.solve();

  ASSERT_NE(nullptr, soln) << dd;
  EXPECT_THAT(*soln, ElementsAre(0));
}

/**     1  1  1  1
 *    +------------+
 *    | 1  1  1  1 |
 *    +------------+
 */
TEST(LatticeTest, SolvableSingle) {
  Lattice<uint8_t, uint8_t> l(
    {0},
    {0, 1, 2, 3},
    [](uint8_t) { return 1; },
    [](uint8_t, uint8_t) { return 1; });

  auto dd = l.debugDump();
  auto soln = l.solve();

  ASSERT_NE(nullptr, soln) << dd;
  EXPECT_THAT(*soln, ElementsAre(0));
}

/**     1  1  1  1
 *    +------------+
 *    | 1  1  0  0 |
 *    | 0  0  1  1 |
 *    +------------+
 */
TEST(LatticeTest, SolvableDisjoint) {
  Lattice<uint8_t, uint8_t> l(
    {0, 1},
    {0, 1, 2, 3},
    [](uint8_t) { return 1; },
    [](uint8_t r, uint8_t c) {
      return r == c / 2 ? 1 : 0;
    });

  auto dd = l.debugDump();
  auto soln = l.solve();

  ASSERT_NE(nullptr, soln) << dd;
  EXPECT_THAT(*soln, UnorderedElementsAre(0, 1));
}

/**     1  1  1  1
 *    +------------+
 *    | 1  1  0  0 |
 *    | 0  1  1  0 |
 *    | 0  0  1  1 |
 *    +------------+
 */
TEST(LatticeTest, SolvableOverlap) {
  Lattice<uint8_t, uint8_t> l(
    {0, 1, 2},
    {0, 1, 2, 3},
    [](uint8_t) { return 1; },
    [](uint8_t r, uint8_t c) {
      uint8_t x = c - r;
      return (x/2 + 1) % 2;
    });

  auto dd = l.debugDump();
  auto soln = l.solve();

  ASSERT_NE(nullptr, soln) << dd;
  EXPECT_THAT(*soln, UnorderedElementsAre(0, 2));
}


/**     2  2  2  2
 *    +------------+
 *    | 1  1  0  0 |
 *    | 0  1  1  0 |
 *    | 0  0  1  1 |
 *    | 1  0  0  1 |
 *    +------------+
 */
TEST(LatticeTest, SolvableOverlapNonBinary) {
  Lattice<uint8_t, uint8_t> l(
    {0, 1, 2, 3},
    {0, 1, 2, 3},
    [](uint8_t) { return 2; },
    [](uint8_t r, uint8_t c) {
      uint8_t x = c - r;
      return (x/2 + 1) % 2;
    });

  auto dd = l.debugDump();
  auto soln = l.solve();

  ASSERT_NE(nullptr, soln) << dd;
  EXPECT_THAT(*soln, UnorderedElementsAre(0, 1, 2, 3));
}
