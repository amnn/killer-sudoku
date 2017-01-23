#include <type_traits>

namespace reversed {

template <typename R>
class Reversed {
  using T = typename std::remove_reference<R>::type;

public:
  using iterator = typename T::reverse_iterator;
  using reverse_iterator = typename T::iterator;

  using const_iterator = typename T::const_reverse_iterator;
  using const_reverse_iterator = typename T::const_iterator;

  Reversed(R &&range)
    : _range {std::forward<R>(range)}
  {}

  iterator begin() { return _range.rbegin(); }
  iterator end()   { return _range.rend(); }

  reverse_iterator rbegin() { return _range.begin(); }
  reverse_iterator rend()   { return _range.end(); }

  const_iterator begin() const { return _range.rbegin(); }
  const_iterator end()   const { return _range.rend(); }

  const_reverse_iterator rbegin() const { return _range.begin(); }
  const_reverse_iterator rend()   const { return _range.end(); }

private:
  R &&_range;
};

template <typename R>
Reversed<R> reverse(R &&range) {
  return Reversed<R> {std::forward<R>(range)};
}

} // namespace reversed
