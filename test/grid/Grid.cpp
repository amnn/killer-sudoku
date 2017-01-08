#include <gtest/gtest.h>

#include <stdexcept>

#include <sstream>

#include <grid/Grid.h>
using namespace grid;

TEST(GridTest, Empty) {
  std::stringstream ss(". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . .");

  auto g = Grid::fromStream(ss);
  for (uint8_t r = 0; r < Grid::HEIGHT; ++r) {
    for (uint8_t c = 0; c < Grid::WIDTH; ++c) {
      EXPECT_FALSE(g.region(r, c).isConstrained);
    }
  }
}

TEST(GridTest, Regions) {
  std::stringstream ss("1 < v < v < v < v"
                       "^ ^ v ^ v ^ v ^ v"
                       "^ ^ 2 ^ v ^ v ^ v"
                       "^ ^ ^ ^ v ^ v ^ v"
                       "^ ^ ^ ^ 3 ^ v ^ v"
                       "^ ^ ^ ^ ^ ^ v ^ v"
                       "^ ^ ^ ^ ^ ^ 4 ^ v"
                       "^ ^ ^ ^ ^ ^ ^ ^ v"
                       "^ ^ ^ ^ ^ ^ ^ ^ 5");

  auto g = Grid::fromStream(ss);

  for (uint8_t r = 0; r < Grid::HEIGHT; ++r) {
    for (uint8_t c = 0; c < Grid::WIDTH; ++c) {
      auto &rgn = g.region(r, c);

      EXPECT_TRUE(rgn.isConstrained);
      EXPECT_EQ(c / 2 + 1, rgn.constraint);
    }
  }

}

TEST(GridTest, InvalidCharacter) {
  std::stringstream ss("a . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . .");

  EXPECT_THROW(Grid::fromStream(ss), std::invalid_argument);
}

TEST(GridTest, TooFew) {
  std::stringstream ss(".");

  EXPECT_THROW(Grid::fromStream(ss), std::invalid_argument);
}

TEST(GridTest, Dangling) {
  std::stringstream ss("^ . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . ."
                       ". . . . . . . . .");

  EXPECT_THROW(Grid::fromStream(ss), std::invalid_argument);
}
