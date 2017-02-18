#include <lattice/NodeIter.h>

#include <lattice/Node.h>

namespace lattice {
namespace detail {

NodeIter::NodeIter(Node *curr)
  : _curr {curr}
{}

bool NodeIter::operator ==(const NodeIter &that) const
{
  return _curr == that._curr;
}

bool NodeIter::operator !=(const NodeIter &that) const
{
  return !(*this == that);
}

Node &NodeIter::operator *() { return *_curr; }
Node *NodeIter::operator ->() { return _curr; }

RHNodeIter::RHNodeIter(Node *curr)
  : NodeIter(curr)
{}

RHNodeIter &RHNodeIter::operator ++()
{
  _curr = _curr->w;
  return *this;
}

RHNodeIter RHNodeIter::operator ++(int)
{
  return RHNodeIter {_curr->w};
}

RVNodeIter::RVNodeIter(Node *curr)
  : NodeIter(curr)
{}

RVNodeIter &RVNodeIter::operator ++()
{
  _curr = _curr->n;
  return *this;
}

RVNodeIter RVNodeIter::operator ++(int)
{
  return RVNodeIter {_curr->n};
}

HNodeIter::HNodeIter(Node *curr)
  : NodeIter(curr)
{}

HNodeIter &HNodeIter::operator ++()
{
  _curr = _curr->e;
  return *this;
}

HNodeIter HNodeIter::operator ++(int)
{
  return HNodeIter {_curr->e};
}

VNodeIter::VNodeIter(Node *curr)
  : NodeIter(curr)
{}

VNodeIter &VNodeIter::operator ++()
{
  _curr = _curr->s;
  return *this;
}

VNodeIter VNodeIter::operator ++(int)
{
  return VNodeIter {_curr->s};
}

} // namespace detail
} // namespace lattice
