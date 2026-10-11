#ifndef PATH
#define PATH

#include <SFML/Graphics.hpp>
#include <map.h>

class Path
{
public:
  void clear_paths(std::vector<sf::Vector3i> &path, Map &map);

  void find_path(std::vector<sf::Vector3i> &path, Map &map);

  void check_coord(Map &map, int x, int y, int z,
                   std::vector<sf::Vector2i> &use_coords,
                   std::vector<sf::Vector3i> &last_step,
                   std::vector<sf::Vector3i> &path);

  bool is_used_coord(int x, int y, std::vector<sf::Vector2i> &coords);

  void dijkstras_algorithm(sf::Vector3i &current,
                          std::vector<bool> &visited,
                          std::vector<sf::Vector3i> &path,
                          Map &map);

  void find_neighbors(sf::Vector3i current,
                     std::vector<sf::Vector3i> &temp,
                     Map &map);
};

#endif /*PATH*/
