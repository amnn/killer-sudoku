#include "Grid.h"

#include <cctype>
#include <cstdio>
#include <sstream>
#include <stdexcept>

#include <lattice/Lattice.h>

namespace grid {

using namespace region;

namespace {
enum struct Dir : uint8_t { U, D, L, R };

enum struct LColType : uint8_t { Row, Col, Box, Cell, Rgn };

struct LatticeCol {
  const LColType type;
  union {
    struct { uint8_t row, col; };
    struct { uint8_t ix, elem; };
    struct { const Region * region; };
  };
};

struct LatticeRow {
  uint8_t row, col, elem;
};

using SolnSpace = lattice::Lattice<LatticeRow, LatticeCol>;
} // anonymous namespace

Grid Grid::fromStream(std::istream &is)
{
  Grid g;

  uint16_t ix = 0;
  auto nbr = [&ix, &g](Dir dir) -> Cell & {
    int16_t to;
    switch (dir) {
      case Dir::U: to = ix - WIDTH; break;
      case Dir::D: to = ix + WIDTH; break;
      case Dir::L: to = ix - 1; break;
      case Dir::R: to = ix + 1; break;
    }

    if (to < 0 || ELEMS <= to) {
      std::stringstream ss;
      ss << "Dangling pointer ["
         << static_cast<int32_t>(ix) << "] ("
         << ix / WIDTH << ", "
         << ix % WIDTH << ") -> ["
         << static_cast<int32_t>(to) << "] ("
         << to / WIDTH << ", "
         << to % WIDTH << ")";
      throw std::invalid_argument(ss.str());
    }

    return g.cell(to);
  };

  char c;
  while (ix < ELEMS && is.get(c)) {
    if (isdigit(c)) {
      uint16_t n = c - '0';

      while (is.get(c) && isdigit(c)) {
        n = n * 10 + (c - '0');
      }

      g.cell(ix++)->setConstraint(n);

      if (!is) break;
    }

    switch (c) {
      case '^': nbr(Dir::U) += g.cell(ix++); break;
      case 'v': nbr(Dir::D) += g.cell(ix++); break;
      case '<': nbr(Dir::L) += g.cell(ix++); break;
      case '>': nbr(Dir::R) += g.cell(ix++); break;

      case '.':
        ix++;
        break;

      case ' ':
      case '\n':
      case '\r':
      case '\t':
        break;

      default: {
        std::stringstream ss;
        ss << "Invalid character '" << c << "'"
           << " near row " << ix / WIDTH << ","
           << " column " << ix % WIDTH;
        throw std::invalid_argument(ss.str());
      }
    }
  }

  if (ix < ELEMS) {
    std::stringstream ss;
    ss << "Only " << ix << " cells populated";
    throw std::invalid_argument(ss.str());
  }

  return g;
}

const region::Region &Grid::region(uint8_t row, uint8_t col)
{
  return region(row * WIDTH + col);
}

const region::Region &Grid::region(uint16_t ix)
{
  return **cell(ix).find();
}

Grid::Soln Grid::solve() {
  std::vector<LatticeRow> rows;
  std::vector<LatticeCol> cols;

  // Construct Row headers
  for (uint8_t r = 0; r < 9; ++r)
    for (uint8_t c = 0; c < 9; ++c)
      for (uint8_t e = 1; e <= 9; ++e)
        rows.push_back({.row = r, .col = c, .elem = e});

  // Construct Column headers for regions.
  {
    std::unordered_set<const Region *> seen;
    for (uint8_t r = 0; r < 9; ++r)
      for (uint8_t c = 0; c < 9; ++c) {
        const Region * rgn = &region(r, c);
        if (rgn->isConstrained && !seen.count(rgn)) {
          seen.insert(rgn);
          LatticeCol lc { .type = LColType::Rgn };
          lc.region = rgn;
          cols.push_back(lc);
        }
      }
  }

  // Construct Column headers for individual cells.
  for (uint8_t r = 0; r < 9; ++r)
    for (uint8_t c = 0; c < 9; ++c) {
      LatticeCol cellLC { .type = LColType::Cell };
      cellLC.row = r;
      cellLC.col = c;
      cols.push_back(cellLC);
    }

  // Construct Column headers for normal sudoku constraints.
  for (uint8_t i = 0; i < 9; ++i)
    for (uint8_t e = 1; e <= 9; ++e) {
      LatticeCol rowLC { .type = LColType::Row };
      rowLC.ix = i;
      rowLC.elem = e;
      cols.push_back(rowLC);

      LatticeCol colLC { .type = LColType::Col };
      colLC.ix = i;
      colLC.elem = e;
      cols.push_back(colLC);

      LatticeCol boxLC { .type = LColType::Box };
      boxLC.ix = i;
      boxLC.elem = e;
      cols.push_back(boxLC);
    }

  SolnSpace ss(
    rows,
    cols,
    [](LatticeCol c) -> uint16_t {
      switch (c.type) {
      case LColType::Rgn:
        return c.region->constraint;
      default:
        return 1u;
      }
    },
    [this](LatticeRow r, LatticeCol c) {
      switch (c.type) {
        case LColType::Rgn: {
          const Region * rgn = &region(r.row, r.col);
          return rgn == c.region ? r.elem : 0;
        }
        case LColType::Row: {
          return r.row == c.ix && r.elem == c.elem ? 1 : 0;
        }
        case LColType::Col: {
          return r.col == c.ix && r.elem == c.elem ? 1 : 0;
        }
        case LColType::Box: {
          uint8_t br  = r.row / 3;
          uint8_t bc  = r.col / 3;
          uint8_t bix = br * 3 + bc;

          return c.ix == bix && r.elem == c.elem ? 1 : 0;
        }
        case LColType::Cell: {
          return c.row == r.row && c.col == r.col ? 1 : 0;
        }
      }
    });

  auto latticeSoln = ss.solve();

  if (!latticeSoln) return nullptr;

  Grid::Soln soln {new std::array<uint8_t, ELEMS>()};
  for (uint16_t i = 0; i < ELEMS; ++i)
    soln->at(i) = 0;

  for (auto entry : *latticeSoln) {
    uint16_t ix = entry.row * WIDTH + entry.col;
    auto &row = soln->at(ix);
    if (row != 0) {
      std::stringstream stream;
      stream
        << "Invalid solution! " << entry.elem << " overwriting " << row
        << " @ (" << entry.col << ", " << entry.row << ")";
      throw std::runtime_error(stream.str());
    }

    row = entry.elem;
  }

  return soln;
}

Cell &Grid::cell(uint16_t ix)
{
  uint8_t row = ix / WIDTH;
  uint8_t col = ix % WIDTH;

  if (!_cells[ix])
    _cells[ix] = _arena.alloc(Region {row, col});

  return *_cells[ix];
}

} // namespace grid
