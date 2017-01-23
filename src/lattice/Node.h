#pragma once

#include <cstdint>
#include <vector>

#include <arena/Arena.h>
#include <lattice/NodeIter.h>

namespace lattice {
namespace detail {

struct Tag {
  struct { uint32_t val; } for_inode;
  struct { const void *ptr; } for_row;
  struct { uint32_t sum, weight; const void *ptr; } for_col;
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

  void hideRow(std::vector<Node *> &hidden);

  std::vector<Node *> pickRow();
  void unPickRow();

  void showRow();

  const Tag &tag();
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
  Node(Node *root, const void *val);

  // Construct Column
  Node(Node *root, uint32_t sum, const void *val);

  // Construct Internal Node
  Node(Node *row, Node *col, uint32_t val);

  Tag _tag;
  Node *r, *c, *n, *s, *w, *e;

  friend struct HNodeIter;
  friend struct VNodeIter;
};

} // namespace detail
} // namespace lattice
