#pragma once
#include <vector>

namespace maze {

enum class CellType { Wall, Path, Start, End, Visited };

struct Point {
  int x, y;

  bool operator==(const Point& other) const;
  bool operator!=(const Point& other) const;
};

struct Direction {
  int dx, dy;
};

class Maze {
 public:
  Maze(int width, int height);
  void Display() const;

  int GetWidth() const { return width_; }
  int GetHeight() const { return height_; }

  CellType GetCell(Point point) const;
  void SetCell(Point point, CellType type);

  bool IsInBound(Point point) const;

  Point GetStartPoint() const;
  void SetStartPoint(Point point);

  Point GetEndPoint() const;
  void SetEndPoint(Point point);

 private:
  int width_, height_;
  Point start_point_, end_point_;
  std::vector<std::vector<CellType>> grid_;
};

}  // namespace maze
