#pragma once

#include <iterator>
#include <type_traits>
#include <vector>

#include <lattice/Node.h>

namespace lattice {

using namespace detail;

template <typename R, typename C>
struct Lattice {

  template <typename SumFn, typename ValFn>
  Lattice(std::vector<R> rs, std::vector<C> cs, SumFn sum, ValFn val)
    : _arena {}
    , _rowTags {rs}
    , _colTags {cs}
    , _root {_arena.alloc()}
  {
    for (const C &c : _colTags)
      _arena.alloc(_root, sum(c), reinterpret_cast<const void *>(&c));

    for (const R &r : _rowTags) {
      Node &row = *_arena.alloc(_root, reinterpret_cast<const void *>(&r));

      for (Node &col : _root->horizRange()) {
        uint32_t nodeVal =
          val(*reinterpret_cast<const R *>(row.tag().for_row.ptr),
              *reinterpret_cast<const C *>(col.tag().for_col.ptr));

        if (nodeVal > 0)
          _arena.alloc(&row, &col, nodeVal);
      }
    }
  }

  /* test */ inline Node *root() const { return _root; }

private:
  Node::Arena _arena;
  const std::vector<R> _rowTags;
  const std::vector<C> _colTags;
  Node * _root;
};

} // namespace lattice
