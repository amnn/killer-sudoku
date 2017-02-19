#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <lattice/Node.h>

#include <vector>

#include <reversed/Reversed.h>

#include "Helpers.h"

using namespace lattice::detail;
using namespace lattice::helpers;
using namespace reversed;

using ::testing::ContainerEq;

TEST(LatticeNodeTest, Root) {
  Node::Arena a;
  auto root = a.alloc();

  EXPECT_EQ(root, root->row());
  EXPECT_EQ(root, root->col());
}

TEST(LatticeNodeTest, Row) {
  Node::Arena a;
  auto root = a.alloc();
  auto row  = a.alloc(root, nullptr);

  EXPECT_EQ(row, row->row());
  EXPECT_EQ(root, row->col());
}

TEST(LatticeNodeTest, Col) {
  Node::Arena a;
  auto root = a.alloc();
  auto col = a.alloc(root, 0u, nullptr);

  EXPECT_EQ(root, col->row());
  EXPECT_EQ(col, col->col());
}

TEST(LatticeNodeTest, EmptyRange) {
  Node::Arena a;
  auto root = a.alloc();
  auto range = root->horizRange();

  EXPECT_EQ(range.begin(), range.end());
}

TEST(LatticeNodeTest, EmptyReverseRange) {
  Node::Arena a;
  auto root = a.alloc();
  auto range = root->horizRange();

  EXPECT_EQ(range.rbegin(), range.rend());
}

TEST(LatticeNodeTest, HorizRange) {
  Node::Arena a;
  auto root = a.alloc();
  std::vector<Node *> nodes {};

  for (uint8_t i = 0; i < 4; ++i)
    nodes.push_back(a.alloc(root, i, nullptr));

  auto range = root->horizRange();
  auto it = range.begin();
  for (auto node : nodes) {
    EXPECT_EQ(node, it->col());
    ++it;
  }

  EXPECT_EQ(range.end(), it);
}

TEST(LatticeNodeTest, ReverseHorizRange) {
  Node::Arena a;
  auto root = a.alloc();
  std::vector<Node *> nodes {};

  for (uint8_t i = 0; i < 4; ++i)
    nodes.push_back(a.alloc(root, i, nullptr));

  auto range = root->horizRange();
  auto it = range.rbegin();
  for (auto node : reverse(nodes)) {
    EXPECT_EQ(node, it->col());
    ++it;
  }

  EXPECT_EQ(range.rend(), it);
}

TEST(LatticeNodeTest, VertRange) {
  Node::Arena a;
  auto root = a.alloc();
  std::vector<Node *> nodes {};

  for (uint8_t i = 0; i < 4; ++i)
    nodes.push_back(a.alloc(root, nullptr));

  auto range = root->vertRange();
  auto it = range.begin();
  for (auto node : nodes) {
    EXPECT_EQ(node, it->row());
    ++it;
  }

  EXPECT_EQ(range.end(), it);
}

TEST(LatticeNodeTest, ReverseVertRange) {
  Node::Arena a;
  auto root = a.alloc();
  std::vector<Node *> nodes {};

  for (uint8_t i = 0; i < 4; ++i)
    nodes.push_back(a.alloc(root, nullptr));

  auto range = root->vertRange();
  auto it = range.rbegin();
  for (auto node : reverse(nodes)) {
    EXPECT_EQ(node, it->row());
    ++it;
  }

  EXPECT_EQ(range.rend(), it);
}

TEST(LatticeNodeTest, ShowHideInRow) {
  Node::Arena a;
  auto root = a.alloc();
  auto row  = a.alloc(root, nullptr);
  std::vector<Node *> nodes {};

  for (uint32_t i = 0; i < 4; ++i) {
    auto col = a.alloc(root, i, nullptr);
    nodes.push_back(a.alloc(row, col, i));
  }

  std::vector<uint32_t>
    all {0, 1, 2, 3},
    removed {0, 2, 3};

  EXPECT_THAT(rvals(row), ContainerEq(all));

  nodes[1]->hideInRow();
  EXPECT_THAT(rvals(row), ContainerEq(removed));

  nodes[1]->showInRow();
  EXPECT_THAT(rvals(row), ContainerEq(all));
}

TEST(LatticeNodeTest, ShowHideInCol) {
  Node::Arena a;
  auto root = a.alloc();
  auto col  = a.alloc(root, 100u, nullptr);
  std::vector<Node *> nodes {};

  for (uint32_t i = 0; i < 4; ++i) {
    auto row = a.alloc(root, nullptr);
    nodes.push_back(a.alloc(row, col, i));
  }

  std::vector<uint32_t>
    all {0, 1, 2, 3},
    removed {0, 2, 3};

  EXPECT_EQ(col->val(), 6);
  EXPECT_THAT(cvals(col), ContainerEq(all));

  nodes[1]->hideInCol();
  EXPECT_EQ(col->val(), 5);
  EXPECT_THAT(cvals(col), ContainerEq(removed));

  nodes[1]->showInCol();
  EXPECT_EQ(col->val(), 6);
  EXPECT_THAT(cvals(col), ContainerEq(all));
}
