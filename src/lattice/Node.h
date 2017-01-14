#pragma once

#include <cstdint>

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

  void hideInRow();
  void showInRow();
  HorizRange horizRange();

  void hideInCol();
  void showInCol();
  VertRange vertRange();

  inline const Tag &tag() { return _tag; }
  inline Node *row()      { return r; }
  inline Node *col()      { return c; }
  inline Node *north()    { return n; }
  inline Node *south()    { return s; }
  inline Node *west()     { return w; }
  inline Node *east()     { return e; }

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
