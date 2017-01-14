#pragma once

namespace lattice {
namespace detail {

struct Node;

struct NodeIter {
  NodeIter(Node *curr);

  bool operator ==(const NodeIter &that) const;
  bool operator !=(const NodeIter &that) const;

  Node &operator *();
  Node *operator ->();

protected:
  Node *_curr;
};

struct HNodeIter : public NodeIter {
  HNodeIter(Node *curr);

  HNodeIter &operator ++();
  HNodeIter operator ++(int);
};

struct VNodeIter : public NodeIter {
  VNodeIter(Node *curr);

  VNodeIter &operator ++();
  VNodeIter operator ++(int);
};

template <typename I>
struct NodeRange {
  NodeRange(Node *begin, Node *end)
    : _begin {begin}
    , _end {end}
  {};

  I begin() const { return I(_begin); };
  I end()   const { return I(_end); };

private:
  Node *const _begin;
  Node *const _end;
};

using HorizRange = NodeRange<HNodeIter>;
using VertRange  = NodeRange<VNodeIter>;

} // namespace detail
} // namespace lattice
