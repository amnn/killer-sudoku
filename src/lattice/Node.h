#pragma once

#include <cstdint>
#include <vector>

#include <arena/Arena.h>
#include <lattice/NodeIter.h>

namespace lattice {
namespace detail {

struct Tag {
  struct { const void *ptr; uint32_t sum; } for_col;
  struct { const void *ptr; } for_row;
};

struct Node {

  using Arena = arena::Arena< Node >;
  friend class arena::Arena< Node >;

  HorizRange horizRange();
  VertRange vertRange();

  void hideInRow();
  void showInRow();

  void hideInCol();
  void showInCol();

  bool isColSatisfiable() const;

  void hideRow();
  void showRow();

  std::vector<Node *> pickRow();
  void unPickRow();

  const uint32_t val() const;
  const Tag &tag() const;
  Node *row();
  Node *col();
  Node *north();
  Node *south();
  Node *west();
  Node *east();

private:

  // Construct Root
  Node();

  // Construct Row
  Node(Node *root, const void *ptr);

  // Construct Column
  Node(Node *root, uint32_t sum, const void *ptr);

  // Construct Internal Node
  Node(Node *row, Node *col, uint32_t val);

  uint32_t _val;
  Tag _tag;
  Node *r, *c, *n, *s, *w, *e;

  friend struct HNodeIter;
  friend struct VNodeIter;
  friend struct RHNodeIter;
  friend struct RVNodeIter;
};

} // namespace detail
} // namespace lattice
