#include <path.h>
#include <iostream>

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
      //std::cout << path.at(i - 1).x << " " << path.at(i - 1).y << std::endl;
      temp.push_back(path.at(i - 1));
      i = path.at(i - 1).z;
    }
    path.erase(path.begin(), path.end());
    for (auto step : temp)
    {
      path.push_back(step);
    }
}

void Path::find_end(std::vector<sf::Vector3i> &path, Map &map)
{
    std::vector<sf::Vector3i> lastStep; // [(x, y, z), (x, y, z),]
    std::vector<sf::Vector2i> usedCoords;
    usedCoords.emplace_back(path.at(0).x, path.at(0).y);
    lastStep.push_back(path.at(0));

    int counter = 0;
    while (counter < 2000)
    {
        int size = lastStep.size();
        for (int i = 0; i < size; ++i)
        {
          auto step = lastStep.at(i);
          check_coord(map, step.x, step.y - 1, path.size() - size + 1 + i, usedCoords, lastStep);
          check_coord(map, step.x + 1, step.y, path.size() - size + 1 + i, usedCoords, lastStep);
          check_coord(map, step.x, step.y + 1, path.size() - size + 1 + i, usedCoords, lastStep);
          check_coord(map, step.x - 1, step.y, path.size() - size + 1 + i, usedCoords, lastStep);
          check_coord(map, step.x + 1, step.y - 1, path.size() - size + 1 + i, usedCoords, lastStep);
          check_coord(map, step.x + 1, step.y + 1, path.size() - size + 1 + i, usedCoords, lastStep);
          check_coord(map, step.x - 1, step.y + 1, path.size() - size + 1 + i, usedCoords, lastStep);
          check_coord(map, step.x - 1, step.y - 1, path.size() - size + 1 + i, usedCoords, lastStep);
        }

        lastStep.erase(lastStep.begin(), lastStep.begin() + size);
        for (auto step : lastStep)
        {
          path.push_back(step);
          if (step.x == map.get_start().x && step.y == map.get_start().y)
          {
              std::cout << "Finished" << std::endl;
              return;
          }
        }
        ++counter;
    }
}

void Path::check_coord(Map &map, int x, int y, int z,
                       std::vector<sf::Vector2i> &use_coords,
                       std::vector<sf::Vector3i> &last_step)
{
  if (!map.is_wall(x, y) && !is_used_coord(x, y, use_coords))
  {
    last_step.emplace_back(x, y, z);
    use_coords.emplace_back(x, y);
  }
}

bool Path::is_used_coord(int x, int y, std::vector<sf::Vector2i> &coords)
{

  // coords.size() returns long unsigned int, cast it to int to compare
  for (int i = 0; i < static_cast<int>(coords.size()); ++i)
  {
    if (coords.at(i).x == x && coords.at(i).y == y)
      return true;
  }
  return false;
}

// finds the shortest path
void Path::dijkstras_algorithm(sf::Vector3i &current,
                               std::vector<bool> &visited,
                               std::vector<sf::Vector3i> &path,
                               Map &map)
{
  std::vector<sf::Vector3i> temp;
  std::vector<sf::Vector3i> frontier;

  current.z = 0;
  frontier.push_back(current);

  while (!frontier.empty())
  {
    // Find the node with the lowest distance.
    int minIndex = 0;
    // frontier.size() returns long unsigned int, cast it to int to compare
    for (int i = 1; i < static_cast<int>(frontier.size()); ++i)
    {
      if (frontier[i].z < frontier[minIndex].z)
        minIndex = i;
    }

    current = frontier[minIndex];
    frontier.erase(frontier.begin() + minIndex);

    int index = current.y * map.get_map_size() + current.x;

    if (visited[index])
      continue;

    visited[index] = true;

    // Stop when the goal is reached.
    if (current.x == path[0].x &&
      current.y == path[0].y)
    {
      path.push_back(current);
      return;
    }

    find_neighbors(current, temp, map);

    for (auto neighbor : temp)
    {
      int neighborIndex =
          neighbor.y * map.get_map_size() + neighbor.x;

      if (visited[neighborIndex])
          continue;

      neighbor.z = current.z + 1;

      frontier.push_back(neighbor);
    }
  }
}

void Path::find_neighbors(sf::Vector3i current,
                          std::vector<sf::Vector3i> &temp,
                          Map &map)
{
  temp.erase(temp.begin(), temp.end());
  if (!map.is_wall(current.x, current.y - 1))
    temp.emplace_back(current.x, current.y - 1, 1);
  if (!map.is_wall(current.x + 1, current.y))
    temp.emplace_back(current.x + 1, current.y, 1);
  if (!map.is_wall(current.x, current.y + 1))
    temp.emplace_back(current.x, current.y + 1, 1);
  if (!map.is_wall(current.x - 1, current.y))
    temp.emplace_back(current.x - 1, current.y, 1);
}
