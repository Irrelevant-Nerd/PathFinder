#ifndef PATH
#define PATH

#include <SFML/Graphics.hpp>
#include <map.h>
#include <vector>
class Path
{
public:
  void clear_paths(std::vector<sf::Vector3i> &path, Map &map);

  bool breadth_first_search(std::vector<sf::Vector3i> &path, Map &map);

  void check_coord(Map &map, int x, int y, int z,
                   std::vector<bool> &used,
                   std::vector<sf::Vector3i> &last_step);

  bool is_used_coord(int x, int y, std::vector<sf::Vector2i> &coords);

  bool dijkstras_algorithm(std::vector<sf::Vector3i> &path, Map &map);

  void find_neighbors(sf::Vector3i current,
                     std::vector<sf::Vector3i> &temp,
                     Map &map);
  std::vector<sf::Vector3i>& get_path() { return path; }

private:
  std::vector<sf::Vector3i> path;
};

#endif /*PATH*/
