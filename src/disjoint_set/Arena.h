#pragma once

#include <memory>

#include "DisjointSet.h"

namespace disjoint_set {

template <typename T>
struct Fst {
  inline T operator()(const T &t, const T &) const
  {
    return t;
  }
};

template < typename T, typename Merger = Fst<T> >
class Arena {
  struct Node {
    Set<T, Merger> s;
    std::unique_ptr<Node> tail;

    Node(T &&val, std::unique_ptr<Node> &&tail)
      : s {std::forward<T>(val)}
      , tail {std::move(tail)}
    {}
  };

public:

  Set<T, Merger> *newSet(T &&val)
  {
    head = std::unique_ptr<Node>(new Node(
      std::forward<T>(val),
      std::move(head)));

    return &head->s;
  }

private:

  std::unique_ptr<Node> head = nullptr;
};
} // namespace disjoint_set
