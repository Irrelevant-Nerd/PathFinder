#include <stdlib.h>
#include <map.h>

Map::Map(int step_size, int map_size, int margin)
: m_step_size {step_size}
, m_map_size {map_size}
, m_margin {margin}
{
  start = sf::Vector2i(0, 0);
  end = sf::Vector2i(map_size - 1, map_size - 1);

  this->rectangles.reserve(m_map_size * m_map_size);
  this->occupants.reserve(m_map_size * m_map_size);
  for (int i = 0; i < m_map_size; ++i)
  {
    for (int j = 0; j < m_map_size; ++j)
    {
      rectangles.push_back(new sf::RectangleShape(sf::Vector2f(m_step_size, m_step_size)));
    }
  }

  init_map();
}

Map::~Map()
{
  for (int i = 0; i < m_map_size; ++i)
  {
    for (int j = 0; j < m_map_size; ++j)
    {
      delete (rectangles.at(i * m_map_size + j));
    }
  }
}

void Map::draw(sf::RenderWindow &window)
{
  for (int i = 0; i < m_map_size; ++i)
  {
    for (int j = 0; j < m_map_size; ++j)
    {
      window.draw(sf::RectangleShape(*rectangles.at(i * m_map_size + j)));
    }
  }
}

void Map::add_path(std::vector <sf::Vector3i> &path)
{
  for (auto step : path)
  {
    int i = step.x;
    int j = step.y;
    occupants.at(i * m_map_size + j) = 2;
    rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Red);
  }
}

void Map::generate_random_map()
{
  srand(3);
  for (int i = 0; i < m_map_size; ++i)
  {
    for (int j = 0; j < m_map_size; ++j)
    {
      int r = rand() % 4; // generates random number from 0 to 3
      rectangles.at(i * m_map_size + j)->setPosition(sf::Vector2f(i * m_step_size + m_margin, j * m_step_size + m_margin));
      int map_area = 2;
      if (r == 0 && !((i < map_area && j < map_area) || (i > m_map_size - map_area && j > m_map_size - map_area)))
      {
        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Black);
        occupants.at(i * m_map_size + j) = 0;
      }
    }
  }
}

void Map::put_wall(int x, int y)
{
  // if out of bounds, return
  if (x + 1 > m_map_size || y + 1 > m_map_size || x < 0 || y < 0)
    return;
  occupants.at(x * m_map_size + y) = 0;
  rectangles.at(x * m_map_size + y)->setFillColor(sf::Color::Black);
}

void Map::remove_wall(int x, int y)
{
  if (x + 1 > m_map_size || y + 1 > m_map_size || x < 0 || y < 0)
    return;

  if (occupants.at(x * m_map_size + y) == 0)
  {
    occupants.at(x * m_map_size + y) = 1;
    rectangles.at(x * m_map_size + y)->setFillColor(sf::Color::White);
  }
}

void Map::remove_walls()
{
  init_map();
}

bool Map::is_wall(int x, int y)
{
  if (x > m_map_size - 1 || y > m_map_size - 1 || y < 0 || x < 0)
      return 1;
  return occupants.at(x * m_map_size + y) == 0;
}

void Map::set_start(int x, int y)
{
  if (x + 1 > m_map_size || y + 1 > m_map_size || x < 0 || y < 0)
    return;
  start = sf::Vector2i(x, y);
  init_map();
}

void Map::set_end(int x, int y)
{
  if (x + 1 > m_map_size || y + 1 > m_map_size || x < 0 || y < 0)
    return;
  end = sf::Vector2i(x, y);
  init_map();
}

void Map::init_map()
{
  for (int i = 0; i < m_map_size; ++i)
  {
    for (int j = 0; j < m_map_size; ++j)
    {
      if (i == start.x && j == start.y) // put the starting cell at the 0, 0 coordinate (very start)
      {
        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Green);
        occupants.push_back(3);
      }
      else if (i == end.x && j == end.y) // put the ending cell at the MAP_SIZE -1, MAP_SIZE -1 coordinate (very end)
      {
        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Red);
        occupants.push_back(4);
      }
      else // the rest of the map is white
      {
        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::White);
        occupants.push_back(1);
      }

      // this is the m_margin, there'll be a thin black line between every cells
      rectangles.at(i * m_map_size + j)->setPosition(sf::Vector2f(i * m_step_size + m_margin, j * m_step_size + m_margin));
      rectangles.at(i * m_map_size + j)->setOutlineThickness(1.0f);
      rectangles.at(i * m_map_size + j)->setOutlineColor(sf::Color::Black);
    }
  }
}
