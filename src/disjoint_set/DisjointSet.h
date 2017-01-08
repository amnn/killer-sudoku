#pragma once

#include <memory>

namespace disjoint_set {

template <typename T, typename Merger>
struct Set {

  // Can't copy or move Sets (to preserve pointers).
  Set(const Set &) = delete;
  Set &operator =(const Set &) = delete;
  Set(Set &&) = delete;
  Set &operator =(Set &&) = delete;

  T &operator *()
  {
    return find()->val;
  }

  T *operator ->()
  {
    return &find()->val;
  }

  Set *find()
  {
    if (this != parent) {
      parent = parent->find();
    }

    return parent;
  }

  Set &operator +=(Set &that)
  {
    auto pthis = this->find();
    auto pthat = that.find();

    if (pthis == pthat)
      return *this;

    Merger m;

    auto rthis = pthis->rank;
    auto rthat = pthat->rank;

    if (rthis < rthat) {
      pthis->parent = pthat;
      pthat->val = m(pthat->val, pthis->val);
    } else {
      if (rthis == rthat)
        pthis->rank++;

      pthat->parent = pthis;
      pthis->val = m(pthis->val, pthat->val);
    }

    return *this;
  }

private:
  // Constructor is private so that it can only be constructed from within a
  // `disjoint_set::Arena`.
  Set(T &&val)
    : parent { this }
    , rank { 0 }
    , val {std::forward<T>(val)}
  {}

  Set *parent;
  uint32_t rank;
  T val;

  template <typename U, typename M>
  friend class Arena;
};

} // namespace disjoint_set
