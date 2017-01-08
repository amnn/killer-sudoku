#include "Grid.h"

#include <cctype>
#include <cstdio>
#include <sstream>
#include <stdexcept>

namespace grid {

using namespace region;

namespace {
  enum Dir { U, D, L, R };
} // anonymous namespace

Grid Grid::fromStream(std::istream &is)
{
  Grid g;

  uint16_t ix = 0;
  auto nbr = [&ix, &g](Dir dir) -> Cell & {
    int16_t to;
    switch (dir) {
    case U: to = ix - WIDTH; break;
    case D: to = ix + WIDTH; break;
    case L: to = ix - 1; break;
    case R: to = ix + 1; break;
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
    case '^': nbr(U) += g.cell(ix++); break;
    case 'v': nbr(D) += g.cell(ix++); break;
    case '<': nbr(L) += g.cell(ix++); break;
    case '>': nbr(R) += g.cell(ix++); break;

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

Cell &Grid::cell(uint16_t ix)
{
  uint8_t row = ix / WIDTH;
  uint8_t col = ix % WIDTH;

  if (!_cells[ix])
    _cells[ix] = _arena.newSet(Region {row, col});

  return *_cells[ix];
}

} // namespace grid
