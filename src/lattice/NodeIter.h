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

struct RHNodeIter : public NodeIter {
  RHNodeIter(Node *curr);

  RHNodeIter &operator ++();
  RHNodeIter operator ++(int);
};

struct RVNodeIter : public NodeIter {
  RVNodeIter(Node *curr);

  RVNodeIter &operator ++();
  RVNodeIter operator ++(int);
};

struct HNodeIter : public NodeIter {
  using Reversed = RHNodeIter;

  HNodeIter(Node *curr);

  HNodeIter &operator ++();
  HNodeIter operator ++(int);
};

struct VNodeIter : public NodeIter {
  using Reversed = RVNodeIter;

  VNodeIter(Node *curr);

  VNodeIter &operator ++();
  VNodeIter operator ++(int);
};

template <typename I>
struct NodeRange {
  using iterator = I;
  using const_iterator = const I;
  using reverse_iterator = typename I::Reversed;
  using const_reverse_iterator = const typename I::Reversed;

  NodeRange(Node *begin, Node *end)
    : _begin {begin}
    , _end {end}
  {};

  I begin() const { return ++I(_begin); };
  I end()   const { return I(_end); };

  typename I::Reversed rbegin() const { return ++typename I::Reversed(_end); }
  typename I::Reversed rend()   const { return typename I::Reversed(_begin); }

private:
  Node *const _begin;
  Node *const _end;
};

using HorizRange = NodeRange<HNodeIter>;
using VertRange  = NodeRange<VNodeIter>;

} // namespace detail
} // namespace lattice
