#include <iostream>
#include <memory>

#include "maze/generators.h"
#include "maze/maze.h"
#include "maze/solver.h"

using namespace maze;

int main() {
  int width, height;
  std::cout << "=== Maze Configuration ===\n";
  std::cout << "Enter width (e.g., 41): ";
  std::cin >> width;
  std::cout << "Enter height (e.g., 21): ";
  std::cin >> height;

  if (width % 2 == 0) width++;
  if (height % 2 == 0) height++;
  auto maze = std::make_unique<Maze>(width, height);

  std::cout << "=== Maze Generator ===\n";
  std::cout << "1. DFS (Long corridors)\n";
  std::cout << "2. Prim (Many branches)\n";
  std::cout << "3. Wilson\n";
  std::cout << "Select algorithm: ";

  int choice;
  std::cin >> choice;

  std::unique_ptr<MazeGenerator> generator;
  switch (choice) {
    case 1:
      generator = std::make_unique<DfsGenerator>();
      break;
    case 2:
      generator = std::make_unique<PrimGenerator>();
      break;
    case 3:
    default:
      generator = std::make_unique<WilsonGenerator>();
      break;
  }

  generator->Generate(*maze);
  maze->Display();

  std::cout << "\nDo you want to solve the maze automatically? (y/n): ";
  char solve_choice;
  std::cin >> solve_choice;

  if (solve_choice == 'y' || solve_choice == 'Y') {
    std::unique_ptr<Solver> solver = std::make_unique<BfsSolver>();
    if (solver->Solve(*maze)) {
      std::cout << "Solved!\n";
    } else {
      std::cout << "No path found.\n";
    }
  }

  return 0;
}
