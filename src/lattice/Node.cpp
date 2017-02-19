#include <lattice/Node.h>

#include <limits>

#include <reversed/Reversed.h>

namespace lattice {
namespace detail {

using namespace reversed;

Node::Node()
  : _val {std::numeric_limits<uint32_t>::max()}
  , _sum {std::numeric_limits<uint32_t>::max()}
  , _ptr {nullptr}
  , r {this}, c {this}
  , n {this}, s {this}
  , w {this}, e {this}
{};

Node::Node(Node *root, const void *ptr)
  : _val {0}
  , _sum {0}
  , _ptr {ptr}
  , r {this},    c {root}
  , n {root->n}, s {root}
  , w {this},    e {this}
{
  showInCol();
}

Node::Node(Node *root, uint32_t sum, const void *ptr)
  : _val {0}
  , _sum {sum}
  , _ptr {ptr}
  , r {root},    c {this}
  , n {this},    s {this}
  , w {root->w}, e {root}
{
  showInRow();
}

Node::Node(Node *row, Node *col, uint32_t val)
  : _val {val}
  , _sum {0}
  , _ptr {nullptr}
  , r {row}, c {col}
  , n {col->n}, s {col}
  , w {row->w}, e {row}
{
  showInRow();
  showInCol();
}

HorizRange Node::horizRange() { return HorizRange {this, this}; }
VertRange  Node::vertRange()  { return VertRange  {this, this}; }

void Node::hideInRow() { w->e = e; e->w = w; }
void Node::showInRow() { w->e = this; e->w = this; }

void Node::hideInCol()
{
  n->s = s;
  s->n = n;

  if (c != this)
    c->_val -= _val;
}

void Node::showInCol()
{
  n->s = this;
  s->n = this;

  if (c != this)
    c->_val += _val;
}

bool Node::isColSatisfiable() const
{
  return c->_sum <= c->_val && c->s != c;
}

void Node::hideRow()
{
  for (Node &i : r->horizRange())
    i.hideInCol();

  r->hideInCol();
}

void Node::showRow()
{
  for (Node &i : r->horizRange())
    i.showInCol();

  r->showInCol();
}

std::vector<Node *> Node::pickRow()
{
  std::vector<Node *> hidden {};

  hideRow();
  hidden.push_back(r);

  for (Node &i : r->horizRange()) {
    auto &sum = i.c->_sum;
    sum -= i._val;

    if (!sum) i.c->hideInRow();

    for (Node &j : i.c->vertRange())
      if (j._val > sum) {
        j.hideRow();
        hidden.push_back(&j);
      }
  }

  return hidden;
}

void Node::unPickRow()
{
  for (Node &i : reverse(r->horizRange())) {
    auto &sum = i.c->_sum;

    if (!sum) c->showInRow();

    sum += i._val;
  }
}

uint32_t Node::val() const { return _val; }
uint32_t Node::sum() const { return _sum; }
const void * Node::ptr() const { return _ptr; }

Node *Node::row()      { return r; }
Node *Node::col()      { return c; }
Node *Node::north()    { return n; }
Node *Node::south()    { return s; }
Node *Node::west()     { return w; }
Node *Node::east()     { return e; }

} // namespace detail
} // namespace lattice
