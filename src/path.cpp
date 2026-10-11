#include <path.h>

void Path::clear_paths(std::vector<sf::Vector3i> &path, Map &map)
{
}

void Path::find_path(std::vector<sf::Vector3i> &path, Map &map)
{

}

void Path::check_coord(Map &map, int x, int y, int z,
                       std::vector<sf::Vector2i> &use_coords,
                       std::vector<sf::Vector3i> &last_step,
                       std::vector<sf::Vector3i> &path)
{

}

bool Path::is_used_coord(int x, int y, std::vector<sf::Vector2i> &coords)
{

}

void Path::dijkstras_algorithm(sf::Vector3i &current,
                              std::vector<bool> &visited,
                              std::vector<sf::Vector3i> &path,
                              Map &map)
{

}

void Path::find_neighbors(sf::Vector3i current,
                          std::vector<sf::Vector3i> &temp,
                          Map &map)
{

}
