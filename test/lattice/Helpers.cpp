#include "Helpers.h"

namespace lattice {
namespace helpers {

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

std::vector<uint32_t> rvals(Node *row) {
  std::vector<uint32_t> vals {};

  for (Node &node : row->horizRange())
    vals.emplace_back(node.val());

  return vals;
}

std::vector<uint32_t> cvals(Node *col) {
  std::vector<uint32_t> vals {};

  for (Node &node : col->vertRange())
    vals.emplace_back(node.val());

  return vals;
}

} // namespace helpers
} // namespace lattice
