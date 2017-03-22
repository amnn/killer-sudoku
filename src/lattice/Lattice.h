#pragma once

#include <iterator>
#include <functional>
#include <memory>
#include <sstream>
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
          val(*reinterpret_cast<const R *>(row.ptr()),
              *reinterpret_cast<const C *>(col.ptr()));

        if (nodeVal > col.sum()) {
          // This whole row will never be satisfiable.
          row.hideRow();
          break;
        }

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
        auto &part = *reinterpret_cast<const R *>(i.row()->ptr());
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

  /* debug */ std::string debugDump() const
  {
    std::stringstream ss;
    ss << "Lattice Debug Output:\n";

    for (Node &col : _root->horizRange())
      ss << col.sum() << ':' << col.val() << '\t';
    ss << '\n';

    for (Node &row : _root->vertRange()) {
      auto rr = row.horizRange();
      auto it = rr.begin();
      for (Node &col : _root->horizRange()) {
        if (it == rr.end()) {
          ss << 'x';
        } else if (it->col() == &col) {
          ss << it->val();
          it++;
        } else {
          ss << 0;
        }
        ss << '\t';
      }

      ss << '\n';
    }
    return ss.str();
  }

private:
  Node::Arena _arena;
  const std::vector<R> _rowTags;
  const std::vector<C> _colTags;
  Node * _root;
};

} // namespace lattice
