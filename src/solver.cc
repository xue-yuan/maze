#include "maze/solver.h"

#include <algorithm>
#include <chrono>
#include <thread>

namespace maze {

bool BfsSolver::Solve(Maze& maze) {
  int width = maze.GetWidth();
  int height = maze.GetHeight();

  Point start_point = maze.GetStartPoint();
  Point end_point = maze.GetEndPoint();

  std::queue<Point> q;
  q.push(start_point);

  std::vector<std::vector<Point>> parent(width,
                                         std::vector<Point>(height, {-1, -1}));
  parent[start_point.x][start_point.y] = start_point;
  std::vector<Direction> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  bool found = false;

  while (!q.empty()) {
    Point curr = q.front();
    q.pop();

    if (curr == end_point) {
      found = true;
      break;
    }

    for (auto& dir : dirs) {
      Point neighbor = {curr.x + dir.dx, curr.y + dir.dy};

      if (maze.IsInBound(neighbor) &&
          maze.GetCell(neighbor) != CellType::Wall &&
          parent[neighbor.x][neighbor.y].x == -1) {
        parent[neighbor.x][neighbor.y] = curr;
        q.push(neighbor);
      }
    }
  }

  if (found) {
    Point p = end_point;
    while (p != start_point) {
      if (p != end_point) {
        maze.SetCell(p, CellType::Visited);
      }
      p = parent[p.x][p.y];
      maze.Display();
    }
    return true;
  }

  return false;
}
}  // namespace maze