#include <iostream>

#include <grid/Grid.h>

int main(int, char **)
{
  auto g = grid::Grid::fromStream(std::cin);

  std::cout << "Solving..." << std::endl;
  auto soln = g.solve();

  if (!soln) {
    std::cout << "No solution!" << std::endl;
    return 0;
  }

  for (uint8_t r = 0; r < 9; ++r) {
    for (uint8_t c = 0; c < 9; ++c) {
      std::cout << static_cast<int>(soln->at(r * 9 + c)) << " ";
    }
    std::cout << '\n';
  }
  std::cout.flush();

  return 0;
}
