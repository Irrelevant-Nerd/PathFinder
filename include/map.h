#ifndef MAP
#define MAP

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class Map
{
public:
  Map(int step_size, int map_size, int margin);
  ~Map();

  void draw(sf::RenderWindow &window);
  void add_path(std::vector <sf::Vector3i> &path);

  void generate_random_map();
  void put_wall(int x, int y);
  void remove_wall(int x, int y);
  void remove_walls();

  bool is_wall(int x, int y);

  void set_start(int x, int y);
  void set_end(int x, int y);
  int generate_randomness(int range);

  int get_map_size() const { return m_map_size; }
  sf::Vector2i get_start() const { return start; }
  sf::Vector2i get_end() const { return end; }

private:
  void init_map();

private:
  int m_step_size {};
  int m_map_size {};
  int m_margin {};

  // this type of vector receieves x and y
  sf::Vector2i start;
  sf::Vector2i end;

  // but this type of vector is just an array
  std::vector<sf::RectangleShape*> rectangles; // this vector is a pointer to sf::RectangleShape data type

  std::vector <int> occupants;

  // hardware anthropy seed
  std::random_device rd;
};

#endif /*MAP*/
