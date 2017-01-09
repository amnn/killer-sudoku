#pragma once

#include <istream>
#include <array>

#include <arena/Arena.h>
#include <region/Region.h>

namespace grid {

using namespace region;

using Cell = disjoint_set::Set<Region, RegionCombiner>;

struct Grid {

  static constexpr uint8_t  WIDTH  = 9;
  static constexpr uint8_t  HEIGHT = 9;
  static constexpr uint16_t ELEMS  = WIDTH * HEIGHT;

  static Grid fromStream(std::istream &is);

  const Region &region(uint16_t ix);
  const Region &region(uint8_t row, uint8_t col);

private:
  Cell &cell(uint16_t ix);

  Cell::Arena _arena;
  std::array<Cell *, ELEMS> _cells {};
};
} // namespace grid
