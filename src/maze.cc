#include "maze/maze.h"

#include <chrono>
#include <iostream>
#include <thread>

bool maze::Point::operator==(const Point& other) const {
  return x == other.x && y == other.y;
}
bool maze::Point::operator!=(const Point& other) const {
  return x != other.x || y != other.y;
}

maze::Maze::Maze(int width, int height)
    : width_(width),
      height_(height),
      grid_(height, std::vector<CellType>(width, CellType::Wall)) {}

void maze::Maze::Display() const {
  std::cout << "\033[2J\033[H";
  for (const auto& row : grid_) {
    for (const auto& cell : row) {
      switch (cell) {
        case CellType::Wall:
          std::cout << "\033[90m█\033[0m";
          break;
        case CellType::Path:
          std::cout << " ";
          break;
        case CellType::Start:
          std::cout << "\033[92mS\033[0m";
          break;
        case CellType::End:
          std::cout << "\033[91mE\033[0m";
          break;
        case CellType::Visited:
          std::cout << "\033[93mX\033[0m";
          break;
      }
    }
    std::cout << "\n";
  }
  std::cout.flush();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

maze::CellType maze::Maze::GetCell(Point point) const {
  return grid_[point.y][point.x];
}

void maze::Maze::SetCell(Point point, CellType type) {
  grid_[point.y][point.x] = type;
}

bool maze::Maze::IsInBound(Point point) const {
  return (point.x > 0 && point.x < width_ - 1 && point.y > 0 &&
          point.y < height_ - 1);
}

maze::Point maze::Maze::GetStartPoint() const { return start_point_; }

maze::Point maze::Maze::GetEndPoint() const { return end_point_; }

void maze::Maze::SetStartPoint(maze::Point point) { start_point_ = point; }

void maze::Maze::SetEndPoint(maze::Point point) { end_point_ = point; }
