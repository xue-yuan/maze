#include <map>
#include <memory>
#include <queue>
#include <vector>

#include "maze.h"

namespace maze {

class Solver {
 public:
  virtual ~Solver() = default;
  virtual bool Solve(Maze& maze) = 0;
};

class BfsSolver : public Solver {
 public:
  bool Solve(Maze& maze) override;
};

}  // namespace maze