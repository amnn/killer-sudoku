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

HorizRange Node::horizRange() { return HorizRange {this->e, this}; }
VertRange  Node::vertRange()  { return VertRange  {this->s, this}; }


void Node::hideInRow() { w->e = e; e->w = w; }
void Node::showInRow() { w->e = this; e->w = this; }

void Node::hideInCol() { n->s = s; s->n = n; }
void Node::showInCol() { n->s = this; s->n = this; }

bool Node::isColSatisfiable() const
{
  auto &t = c->_tag.for_col;
  return t.sum <= t.weight && c->s != c;
}

void Node::hideRow(std::vector<Node *> &hidden)
{
  for (Node &i : r->horizRange()) {
    auto &t = i.c->_tag.for_col;
    t.weight -= i._tag.for_inode.val;
    i.hideInCol();
  }

  hidden.push_back(r);
}

std::vector<Node *> Node::pickRow()
{
  std::vector<Node *> hidden {};

  hideRow(hidden);

  for (Node &i : r->horizRange()) {
    auto &t = i.c->_tag.for_col;
    t.sum -= i._tag.for_inode.val;

    for (Node &j : i.c->vertRange())
      if (j._tag.for_inode.val > t.sum)
        j.hideRow(hidden);
  }

  return hidden;
}

void Node::unPickRow()
{
  for (Node &i : r->horizRange())
    i.c->_tag.for_col.sum += i._tag.for_inode.val;
}

void Node::showRow()
{
  for (Node &i : r->horizRange()) {
    auto &t = i.c->_tag.for_col;
    t.weight += i._tag.for_inode.val;
    i.showInCol();
  }
}

const Tag &Node::tag() { return _tag; }
Node *Node::row()      { return r; }
Node *Node::col()      { return c; }
Node *Node::north()    { return n; }
Node *Node::south()    { return s; }
Node *Node::west()     { return w; }
Node *Node::east()     { return e; }

} // namespace detail
} // namespace lattice
