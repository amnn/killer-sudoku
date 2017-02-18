#pragma once

#include <iterator>
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

#include <lattice/Node.h>
#include <reversed/Reversed.h>

namespace lattice {

using namespace detail;
using namespace reversed;

template <typename R, typename C>
struct Lattice {

  using Soln  = std::unique_ptr< std::vector<R> >;
  using SumFn = std::function<uint32_t(const C &)>;
  using ValFn = std::function<uint32_t(const R &, const C &)>;

  Lattice(std::vector<R> rs, std::vector<C> cs, SumFn sum, ValFn val)
    : _arena {}
    , _rowTags {std::move(rs)}
    , _colTags {std::move(cs)}
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

  Soln solve() {
    if (_root->west() == _root)
      return std::make_unique< std::vector<R> >(0);

    auto col = _root->west();

    if (!col->isColSatisfiable())
      return nullptr;

    for (Node &i : col->vertRange()) {
      auto hidden = i.pickRow();

      if (auto soln = solve()) {
        auto &part = *reinterpret_cast<R *>(i.row()->tag().for_row.ptr);
        soln->push_back(part);
        return soln;
      }

      for (Node *j : reverse(hidden))
        j->showRow();

      i.unPickRow();
    }

    return nullptr;
  }

  /* test */ inline Node *root() const { return _root; }

private:
  Node::Arena _arena;
  const std::vector<R> _rowTags;
  const std::vector<C> _colTags;
  Node * _root;
};

} // namespace lattice
