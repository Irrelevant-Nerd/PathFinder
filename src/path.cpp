#include <path.h>
#include <iostream>

#include <queue>
#include <limits>
#include <algorithm>
#include <functional>


// find the path using breadth-first search (BFS)
bool Path::breadth_first_search(std::vector<sf::Vector3i> &path, Map &map)
{
  std::vector<sf::Vector3i> last_step; // [(x, y, z), (x, y, z),]

  const int map_size = map.get_map_size();
  std::vector<bool> used(map_size * map_size, false);
  used[path.at(0).x * map_size + path.at(0).y] = true;

  last_step.push_back(path.at(0));

  int counter = 0;
  while (counter < 2000)
  {
    int size = last_step.size();
    for (int i = 0; i < size; ++i)
    {
      // make this follow the no-corner-cutting rule like dijkstras algorithm

      auto step = last_step.at(i);
      int z = path.size() - size + 1 + i;

      // straight moves stay as they are
      check_coord(map, step.x,     step.y - 1, z, used, last_step);
      check_coord(map, step.x + 1, step.y,     z, used, last_step);
      check_coord(map, step.x,     step.y + 1, z, used, last_step);
      check_coord(map, step.x - 1, step.y,     z, used, last_step);

      // diagonals only if both side cells are open
      if (!map.is_wall(step.x + 1, step.y) && !map.is_wall(step.x, step.y - 1))
        check_coord(map, step.x + 1, step.y - 1, z, used, last_step);
      if (!map.is_wall(step.x + 1, step.y) && !map.is_wall(step.x, step.y + 1))
        check_coord(map, step.x + 1, step.y + 1, z, used, last_step);
      if (!map.is_wall(step.x - 1, step.y) && !map.is_wall(step.x, step.y + 1))
        check_coord(map, step.x - 1, step.y + 1, z, used, last_step);
      if (!map.is_wall(step.x - 1, step.y) && !map.is_wall(step.x, step.y - 1))
        check_coord(map, step.x - 1, step.y - 1, z, used, last_step);
    }

    last_step.erase(last_step.begin(), last_step.begin() + size);
    for (auto step : last_step)
    {
      path.push_back(step);
      if (step.x == map.get_start().x && step.y == map.get_start().y) return true;
    }
    ++counter;
  }

  // return false if no end cell found
  return false;
}

// turns visited cells to an actual route (not necessarily clearing paths as the name suggests)
void Path::clear_paths(std::vector<sf::Vector3i> &path, Map &map)
{
    std::vector<sf::Vector3i> temp;
    sf::Vector3i start;
    for (auto step : path)
    {
      if (step.x == map.get_start().x && step.y == map.get_start().y)
      {
        start = step;
    }
    }
    temp.push_back(start);
    int i = start.z;
    while (i > 0)
    {
      temp.push_back(path.at(i - 1));
      i = path.at(i - 1).z;
    }
    path.erase(path.begin(), path.end());
    for (auto step : temp)
    {
      path.push_back(step);
    }
}

// looks for one neighbor cell to decide whether it should be explored next by the search
void Path::check_coord(Map &map, int x, int y, int z,
                       std::vector<bool> &used,
                       std::vector<sf::Vector3i> &last_step)
{
  // also catches out of bounds, so check it first
  if (map.is_wall(x, y))
    return;

  // turns a cell x and y position into a single number to used as an index
  int index = x * map.get_map_size() + y;
  if (used[index]) // used the index to look whether the cell was already visited
    return;        // true - already visited, false - not visited yet

  used[index] = true;
  last_step.emplace_back(x, y, z); // adds a new entry to the end of last_step
}

// finds the path using dijkstras_algorithm
bool Path::dijkstras_algorithm(std::vector<sf::Vector3i> &path, Map &map)
{
  path.clear();

  const int size = map.get_map_size();
  const int INF = std::numeric_limits<int>::max();

  std::vector<int> dist(size * size, INF); // cheapest known cost to each cell
  std::vector<int> parent(size * size, -1); // which cell we came from

  // (cost, cell index). greater<> makes the cheapest come out first
  using Item = std::pair<int, int>;
  std::priority_queue<Item, std::vector<Item>, std::greater<Item>> queue;

  int startIndex = map.get_start().x * size + map.get_start().y;
  int endIndex = map.get_end().x * size + map.get_end().y;

  dist[startIndex] = 0;
  queue.push({0, startIndex});

  // dx, dy, cost (straight = 10, diagonal = 14)
  const int moves[8][3] = {{0, -1, 10}, {1, 0, 10}, {0, 1, 10}, {-1, 0, 10},
                           {1, -1, 14}, {1, 1, 14}, {-1, 1, 14}, {-1, -1, 14}};

  while (!queue.empty())
  {
    auto [cost, index] = queue.top();
    queue.pop();

    if (cost > dist[index]) // old entry, a cheaper one was already found
      continue;
    if (index == endIndex)
      break;

    int x = index / size;
    int y = index % size;

    for (auto &m : moves)
    {
      int nx = x + m[0];
      int ny = y + m[1];

      if (map.is_wall(nx, ny)) // is_wall also covers out of bounds
        continue;

      // no cutting corners: a diagonal needs both side cells open
      if (m[2] == 14 && (map.is_wall(nx, y) || map.is_wall(x, ny)))
        continue;

      int nIndex = nx * size + ny;
      int newCost = cost + m[2];

      if (newCost < dist[nIndex])
      {
        dist[nIndex] = newCost;
        parent[nIndex] = index;
        queue.push({newCost, nIndex});
      }
    }
  }

  if (dist[endIndex] == INF) // never reached the end
    return false;

  // walk back from the end to the start, then flip it
  for (int i = endIndex; i != -1; i = parent[i])
    path.emplace_back(i / size, i % size, 0);
  std::reverse(path.begin(), path.end());

  return true;
}
