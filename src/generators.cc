#include "maze/generators.h"

#include <algorithm>
#include <vector>

maze::MazeGenerator::MazeGenerator() : rng_(std::random_device{}()) {}

void maze::DfsGenerator::Generate(Maze& maze) {
  Point start_point = {1, 1};
  Point farthest_point = {start_point.x, start_point.y};
  int max_depth = 0;

  std::vector<Direction> dirs = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};

  auto backtrack = [&](auto& self, int x, int y, int current_depth) -> void {
    maze.SetCell({x, y}, CellType::Path);
    maze.Display();

    std::shuffle(dirs.begin(), dirs.end(), this->rng_);

    for (auto& dir : dirs) {
      Point neighbor = {x + dir.dx, y + dir.dy};

      if (maze.IsInBound(neighbor) &&
          maze.GetCell(neighbor) == CellType::Wall) {
        maze.SetCell({(x + neighbor.x) / 2, (y + neighbor.y) / 2},
                     CellType::Path);
        maze.Display();

        if (current_depth > max_depth) {
          max_depth = current_depth;
          farthest_point = neighbor;
        }

        self(self, neighbor.x, neighbor.y, current_depth + 1);
      }
    }
  };

  backtrack(backtrack, start_point.x, start_point.y, max_depth);
  maze.SetCell(start_point, CellType::Start);
  maze.SetCell(farthest_point, CellType::End);

  maze.SetStartPoint(start_point);
  maze.SetEndPoint(farthest_point);
}

void maze::PrimGenerator::Generate(Maze& maze) {
  Point start_point = {1, 1};
  Point farthest_point = {start_point.x, start_point.y};
  int max_depth = 0;

  maze.SetCell(start_point, CellType::Path);
  maze.Display();

  struct Wall {
    int wx, wy, tx, ty, current_depth;
  };

  std::vector<Wall> wall_list = {};

  auto add_walls = [&](int x, int y, int current_depth) -> void {
    std::vector<Direction> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (auto& dir : dirs) {
      int wx = x + dir.dx;
      int wy = y + dir.dy;
      int tx = x + dir.dx * 2;
      int ty = y + dir.dy * 2;

      if (maze.IsInBound({tx, ty}) &&
          maze.GetCell({tx, ty}) == CellType::Wall) {
        wall_list.push_back({wx, wy, tx, ty, current_depth + 1});
      }
    }
  };

  add_walls(start_point.x, start_point.y, max_depth);

  while (!wall_list.empty()) {
    std::uniform_int_distribution<int> dist(0, wall_list.size() - 1);
    int index = dist(this->rng_);

    Wall w = wall_list[index];
    if (maze.GetCell({w.tx, w.ty}) == CellType::Wall) {
      maze.SetCell({w.wx, w.wy}, CellType::Path);
      maze.SetCell({w.tx, w.ty}, CellType::Path);
      maze.Display();

      add_walls(w.tx, w.ty, w.current_depth);
      if (w.current_depth > max_depth) {
        max_depth = w.current_depth;
        farthest_point = {w.tx, w.ty};
      }
    }
    wall_list[index] = wall_list.back();
    wall_list.pop_back();
  }

  maze.SetCell(start_point, CellType::Start);
  maze.SetCell(farthest_point, CellType::End);

  maze.SetStartPoint(start_point);
  maze.SetEndPoint(farthest_point);
}

void maze::WilsonGenerator::Generate(Maze& maze) {
  std::vector<std::vector<Direction>> grid_dirs(
      maze.GetWidth(), std::vector<Direction>(maze.GetHeight(), {0, 0}));
  std::vector<std::vector<bool>> is_in_maze(
      maze.GetWidth(), std::vector<bool>(maze.GetHeight(), false));

  std::vector<Point> unvisited_rooms;
  for (int y = 1; y < maze.GetHeight(); y += 2) {
    for (int x = 1; x < maze.GetWidth(); x += 2) {
      unvisited_rooms.push_back({x, y});
    }
  }

  std::uniform_int_distribution<int> dist(0, unvisited_rooms.size() - 1);
  int index = dist(this->rng_);
  Point seed = unvisited_rooms[index];

  is_in_maze[seed.x][seed.y] = true;
  maze.SetCell({seed.x, seed.y}, CellType::Path);
  maze.Display();

  unvisited_rooms[index] = unvisited_rooms.back();
  unvisited_rooms.pop_back();

  std::vector<Direction> dirs = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};

  while (!unvisited_rooms.empty()) {
    std::uniform_int_distribution<int> start_dist(0,
                                                  unvisited_rooms.size() - 1);
    Point start_node = unvisited_rooms[start_dist(this->rng_)];
    Point curr = start_node;

    while (!is_in_maze[curr.x][curr.y]) {
      std::uniform_int_distribution<int> dir_dist(0, 3);
      Direction d = dirs[dir_dist(this->rng_)];

      int nx = curr.x + d.dx;
      int ny = curr.y + d.dy;

      if (maze.IsInBound({nx, ny})) {
        grid_dirs[curr.x][curr.y] = d;
        curr = {nx, ny};
      }
    }

    Point walk = start_node;
    while (!is_in_maze[walk.x][walk.y]) {
      is_in_maze[walk.x][walk.y] = true;

      auto it = std::find(unvisited_rooms.begin(), unvisited_rooms.end(), walk);
      if (it != unvisited_rooms.end()) {
        *it = unvisited_rooms.back();
        unvisited_rooms.pop_back();
      }

      Direction d = grid_dirs[walk.x][walk.y];

      maze.SetCell({walk.x, walk.y}, CellType::Path);
      maze.SetCell({walk.x + d.dx / 2, walk.y + d.dy / 2}, CellType::Path);

      maze.Display();
      walk = {walk.x + d.dx, walk.y + d.dy};
    }
  }

  maze.SetCell({1, 1}, CellType::Start);
  maze.SetCell({maze.GetWidth() - 2, maze.GetHeight() - 2}, CellType::End);

  maze.SetStartPoint({1, 1});
  maze.SetEndPoint({maze.GetWidth() - 2, maze.GetHeight() - 2});
}
