#pragma once

#include <sstream>
#include <stdexcept>

#include <disjoint_set/DisjointSet.h>

namespace region {

  struct Region {
    const bool isConstrained;
    const uint8_t constraint;
    const uint8_t row;
    const uint8_t col;

    Region(uint8_t constraint, uint8_t row, uint8_t col)
      : isConstrained { true }
      , constraint { constraint }
      , row { row }
      , col { col }
    {}

    Region(uint8_t row, uint8_t col)
      : isConstrained { false }
      , constraint { 0 }
      , row { row }
      , col { col }
    {}

    bool operator ==(const Region &that) const {
      return memcmp(this, &that, sizeof(Region)) == 0;
    }
  };

  struct RegionCombiner {
    inline Region operator()(const Region &x, const Region &y) const
    {
      if (x.isConstrained && y.isConstrained) {
        std::stringstream ss;
        ss << "Two constraints in region: "
           << x.constraint
           << " at (" << x.col << ", " << x.row << ")"
           << " and "
           << y.constraint
           << " at (" << y.col << ", " << y.row << ")";

        throw std::invalid_argument(ss.str());
      }

      return x.isConstrained
        ? x
        : y;
    }
  };

  using RegionSet = disjoint_set::Set<Region, RegionCombiner>;

}
