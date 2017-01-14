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
