#include <lattice/Node.h>

namespace lattice {
namespace detail {

Node::Node()
  : _tag {{0}}
  , r {this}, c {this}
  , n {this}, s {this}
  , w {this}, e {this}
{};

Node::Node(Node *root, const void *val)
  : _tag {{0}}
  , r {this},    c {root}
  , n {root->n}, s {root}
  , w {this},    e {this}
{
  showInCol();
  _tag.for_row = {
    .ptr = val,
  };
}

Node::Node(Node *root, uint32_t sum, const void *val)
  : _tag {{0}}
  , r {root},    c {this}
  , n {this},    s {this}
  , w {root->w}, e {root}
{
  showInRow();
  _tag.for_col = {
    .sum    = sum,
    .weight = 0,
    .ptr    = val,
  };
}

Node::Node(Node *row, Node *col, uint32_t val)
  : _tag {{0}}
  , r {row}, c {col}
  , n {col->n}, s {col}
  , w {row->w}, e {row}
{
  showInRow();
  showInCol();
  _tag.for_inode = {
    .val = val,
  };
}

void Node::hideInRow() { w->e = e; e->w = w; }
void Node::showInRow() { w->e = this; e->w = this; }

void Node::hideInCol() { n->s = s; s->n = n; }
void Node::showInCol() { n->s = this; s->n = this; }

HorizRange Node::horizRange() { return HorizRange {this->e, this}; }
VertRange  Node::vertRange()  { return VertRange  {this->s, this}; }

} // namespace detail
} // namespace lattice
