#ifndef PATH
#define PATH

#include <SFML/Graphics.hpp>
#include <map.h>
#include <vector>
class Path
{
public:
  bool breadth_first_search(std::vector<sf::Vector3i> &path, Map &map);

  // both of this methods belongs to breadth_first_search only
  void clear_paths(std::vector<sf::Vector3i> &path, Map &map);
  void check_coord(Map &map, int x, int y, int z,
                   std::vector<bool> &used,
                   std::vector<sf::Vector3i> &last_step);

  bool dijkstras_algorithm(std::vector<sf::Vector3i> &path, Map &map);

  std::vector<sf::Vector3i>& get_path() { return path; }

private:
  std::vector<sf::Vector3i> path;
};

#endif /*PATH*/
