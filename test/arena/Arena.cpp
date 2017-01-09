#include <gtest/gtest.h>

#include <arena/Arena.h>

using namespace arena;

namespace {
struct Traced {

  Traced(uint32_t &moves, uint32_t &destroys)
    : moves {moves}
    , destroys {destroys}
  {}

  Traced(Traced &&traced)
    : moves {traced.moves}
    , destroys {traced.destroys}
  {
    moves++;
  }

  Traced(const Traced &) = delete;
  Traced &operator =(const Traced &) = delete;

  ~Traced()
  {
    destroys++;
  }

private:
  uint32_t &moves, &destroys;
};
} // anonymous namespace

TEST(ArenaTest, Alloc) {
  Arena<int32_t> a;
  int32_t *i = a.alloc(42);
  EXPECT_EQ(42, *i);
}

TEST(ArenaTest, Lifetimes) {
  uint32_t moves = 0, destroys = 0;
  {
    Arena<Traced> a;
    a.alloc(moves, destroys);
    a.alloc(moves, destroys);

    EXPECT_EQ(moves, destroys);
  }

  EXPECT_EQ(moves + 2, destroys);
}
