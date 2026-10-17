#include <map.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>

Map::Map(int step_size, int map_size, int margin)
: m_step_size {step_size}
, m_map_size {map_size}
, m_margin {margin}
{
  start = sf::Vector2i(0, 0);
  end = sf::Vector2i(map_size - 1, map_size - 1);

  // resize = only holds the amount of elements stored
  // reserve = only gives a memory for amount of elements that is gonna be stored

  // rectangles stores pointers, so it must start empty, use reserve here
  this->rectangles.reserve(m_map_size * m_map_size);

  // resize stores integers, so allocate elements right away
  this->occupants.resize(m_map_size * m_map_size);

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

// DRAW WALLS
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

// ADD PATH
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

// GENERATE MAP RANDOMLY
void Map::generate_random_map()
{
  // if theres an existing wall
  if(std::find(occupants.begin(), occupants.end(), 0) != occupants.end())
  {
    std::cout << "generate map again\n";
    for (int i = 0; i < m_map_size; ++i)
    {
      for (int j = 0; j < m_map_size; ++j)
      {
        // skip the start cell and end cell
        if(occupants.at(i * m_map_size + j) == 3 || occupants.at(i * m_map_size + j) == 4) continue;

        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Black);
        occupants.at(i * m_map_size + j) = 1;
      }
    }
  }
  else std::cout << "generate map for the first time\n";

  for (int i = 0; i < m_map_size; ++i)
  {
    for (int j = 0; j < m_map_size; ++j)
    {
      // skip the start cell and end cell
      if(occupants.at(i * m_map_size + j) == 3 || occupants.at(i * m_map_size + j) == 4) continue;

      int r = generate_randomness(4); // generates random number from 0 to 3
      rectangles.at(i * m_map_size + j)->setPosition(sf::Vector2f(i * m_step_size + m_margin, j * m_step_size + m_margin));
      int map_area = 2;
      if (r == 0 && !((i < map_area && j < map_area) || (i > m_map_size - map_area && j > m_map_size - map_area)))
      {
        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Yellow);
        occupants.at(i * m_map_size + j) = 0;
      }
    }
  }
}

// PUT A WALL
void Map::put_wall(int x, int y)
{
  // if out of bounds, return
  if (x + 1 > m_map_size || y + 1 > m_map_size || x < 0 || y < 0)
    return;

  // the cell that we are replacing should not be a start cell (3) or end cell (4)
  if(occupants.at(x * m_map_size + y) != 3 && occupants.at(x * m_map_size + y) != 4 )
  {
    occupants.at(x * m_map_size + y) = 0;
    rectangles.at(x * m_map_size + y)->setFillColor(sf::Color::Yellow);
  }
  else std::cout << "You cannot put a wall on a start or end cell\n";
}

// REMOVE ALL WALLS
void Map::remove_wall(int x, int y)
{
  if (x + 1 > m_map_size || y + 1 > m_map_size || x < 0 || y < 0)
    return;

  if (occupants.at(x * m_map_size + y) == 0)
  {
    occupants.at(x * m_map_size + y) = 1;
    rectangles.at(x * m_map_size + y)->setFillColor(sf::Color::Black);
  }
}

// REMOVE WALLS
void Map::remove_walls()
{
  init_map();
}

// CHECK IF THE CELL IS A WALL
bool Map::is_wall(int x, int y)
{
  if (x > m_map_size - 1 || y > m_map_size - 1 || y < 0 || x < 0)
      return 1;
  return occupants.at(x * m_map_size + y) == 0;
}

// SET START CELL RIGHT AFTER INITIALIZING A MAP
void Map::set_start(int x, int y)
{
  if (x + 1 > m_map_size || y + 1 > m_map_size || x < 0 || y < 0)
    return;
  start = sf::Vector2i(x, y);
  init_map();
}

// SET END CELL RIGHT AFTER INITIALIZING A MAP
void Map::set_end(int x, int y)
{
  if (x + 1 > m_map_size || y + 1 > m_map_size || x < 0 || y < 0)
    return;
  end = sf::Vector2i(x, y);
  init_map();
}

int Map::generate_randomness(int range)
{
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, range-1);

  return distrib(gen);
}

// INITIALIZE MAP AFTER CONSTRUCTING MAP
void Map::init_map()
{
  for (int i = 0; i < m_map_size; ++i)
  {
    for (int j = 0; j < m_map_size; ++j)
    {
      if (i == start.x && j == start.y) // put the starting cell at the 0, 0 coordinate (very start)
      {
        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Green);
        // position = value
        occupants.at(i * m_map_size + j) = 3;
      }
      else if (i == end.x && j == end.y) // put the ending cell at the MAP_SIZE -1, MAP_SIZE -1 coordinate (very end)
      {
        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Red);
        occupants.at(i * m_map_size + j) = 4;
      }
      else // the rest of the map is black
      {
        rectangles.at(i * m_map_size + j)->setFillColor(sf::Color::Black);
        occupants.at(i * m_map_size + j) = 1;
      }

      // this is the m_margin, there'll be a thin white line between every cells
      rectangles.at(i * m_map_size + j)->setPosition(sf::Vector2f(i * m_step_size + m_margin, j * m_step_size + m_margin));
      rectangles.at(i * m_map_size + j)->setOutlineThickness(1.0f);
      rectangles.at(i * m_map_size + j)->setOutlineColor(sf::Color::White);
    }
  }
}
