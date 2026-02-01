#pragma once
#include <random>

#include "maze/maze.h"

namespace maze {

class MazeGenerator {
 public:
  MazeGenerator();
  virtual ~MazeGenerator() = default;

  virtual void Generate(Maze& maze) = 0;

 protected:
  std::mt19937 rng_;
};

class DfsGenerator : public MazeGenerator {
 public:
  void Generate(Maze& maze) override;
};

class PrimGenerator : public MazeGenerator {
 public:
  void Generate(Maze& maze) override;
};

class WilsonGenerator : public MazeGenerator {
 public:
  void Generate(Maze& maze) override;
};

}  // namespace maze
