#include <gtest/gtest.h>

#include <lattice/Lattice.h>
using namespace lattice;

#include <vector>

TEST(LatticeTest, Init) {
  std::vector<uint8_t> tags {0, 1, 2, 3, 4};
  Lattice<uint8_t, uint8_t> lattice(
    tags, tags,
    [](uint8_t) { return 0; },
    [](uint8_t r, uint8_t c) {
      return (r + c) % 2
        ? (r + c)
        : 0;
    });

  std::vector< std::vector<uint32_t> >
    vals {{1, 3}, {1, 3, 5}, {3, 5}, {3, 5, 7}, {5, 7}};

  auto valsIt = vals.begin();
  for (Node &row : lattice.root()->vertRange()) {
    EXPECT_NE(vals.end(), valsIt);
    auto rowIt = valsIt->begin();

    for (Node &node : row.horizRange()) {
      EXPECT_NE(valsIt->end(), rowIt);

      EXPECT_EQ(node.tag().for_inode.val, *rowIt++);
    }
    ++valsIt;
  }
}

TEST(LatticeTest, UnsolvableEmpty) {

}

TEST(LatticeTest, UnsolvableZero) {

}

TEST(LatticeTest, SolvableDisjoint) {

}

TEST(LatticeTest, SolvableOverlap) {

}

TEST(LatticeTest, SolvableOverlapNonBinary) {

}
