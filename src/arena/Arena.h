#pragma once

#include <memory>

namespace arena {

template <typename T>
class Arena {
  struct Node {
    T elem;
    std::unique_ptr<Node> tail;

    template <typename ...Args>
    Node(std::unique_ptr<Node> &&tail, Args && ...args)
      : elem  {std::forward<Args>(args)...}
      , tail  {std::move(tail)}
    {}
  };

public:

  template <typename ...Args>
  T *alloc(Args && ...args)
  {
    head = std::unique_ptr<Node>(new Node(
      std::move(head),
      std::forward<Args>(args)...));

    return &head->elem;
  }

private:
  std::unique_ptr<Node> head = nullptr;
};
} // namespace arena
