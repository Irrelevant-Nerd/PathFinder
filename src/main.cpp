#include <SFML/Graphics.hpp>
#include <iostream>
#include <map.h>
#include <path.h>

void handle_keys(auto* key, int MAP_SIZE, int MARGIN, Map &map, sf::RenderWindow &window)
{
  // removes all walls
  if(key->code == sf::Keyboard::Key::C)
  {
    map.remove_walls();
  }
  // randomly generates a wall
  if(key->code == sf::Keyboard::Key::R)
  {
    map.generate_random_map();
  }

  // start a path
  if (key->code == sf::Keyboard::Key::B)
    path.emplace_back(map.get_end().x, map.get_end().y, 0);
    find_end(path, map);
    clear_paths(path, map);
    map.add_path(path);
    path_done = true;{
  }

  // randomly setting a start cell across the map
  if(key->code == sf::Keyboard::Key::S)
  {
    int mx = sf::Mouse::getPosition(window).x - MARGIN;
    int my = sf::Mouse::getPosition(window).y - MARGIN;
    // if map is not out of bounds
    if (!(mx < 0 || my < 0 || mx > 1280 - MARGIN || my > 1280 - MARGIN))
    {
      int x = mx / ((1280 - 2 * MARGIN) / MAP_SIZE);
      int y = my / ((1280 - 2 * MARGIN) / MAP_SIZE);
      map.set_start(x, y);
    }
  }

  // randomly setting an end cell across the map
  if(key->code == sf::Keyboard::Key::E)
  {
    // x and y coordinate of mouse
    int mx = sf::Mouse::getPosition(window).x - MARGIN;
    int my = sf::Mouse::getPosition(window).y - MARGIN;

    // mouse is not out of bounds
    if (!(mx < 0 || my < 0 || mx > 1280 - MARGIN || my > 1280 - MARGIN))
    {
      int x = mx / ((1280 - 2 * MARGIN) / MAP_SIZE);
      int y = my / ((1280 - 2 * MARGIN) / MAP_SIZE);
      map.set_end(x, y);
    }
  }

  if(key->code == sf::Keyboard::Key::R)
  {
    map.generate_random_map();
  }

}
// handles the mouse
void handle_mouse(int MAP_SIZE, int MARGIN, Map &map, sf::RenderWindow &window)
{
  // puts a wall on a cell by right clicking
  if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
  {
    int mx = sf::Mouse::getPosition(window).x - MARGIN;
    int my = sf::Mouse::getPosition(window).y - MARGIN;
    if (!(mx < 0 || my < 0 || mx > 1280 - MARGIN || my > 1280 - MARGIN))
    {
      int x = mx / ((1280 - 2 * MARGIN) / MAP_SIZE);
      int y = my / ((1280 - 2 * MARGIN) / MAP_SIZE);
      map.put_wall(x, y);
    }
  }

  // removes a wall on a cell by left clicking
  else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
  {
    int mx = sf::Mouse::getPosition(window).x - MARGIN;
    int my = sf::Mouse::getPosition(window).y - MARGIN;
    if (!(mx < 0 || my < 0 || mx > 1280 - MARGIN || my > 1280 - MARGIN))
    {
      int x = mx / ((1280 - 2 * MARGIN) / MAP_SIZE);
      int y = my / ((1280 - 2 * MARGIN) / MAP_SIZE);
      map.remove_wall(x, y);
    }
  }
}

int main()
{
  // CONSTANTS
  constexpr int wWIDTH = 1080;
  constexpr int wHEIGHT = 1080;

  int MAP_SIZE = 50;
  int MARGIN = 50;

  sf::RenderWindow window(sf::VideoMode({wWIDTH, wHEIGHT}), "Path Finder");

  Map map((1280 - MARGIN * 2) / MAP_SIZE, MAP_SIZE, MARGIN);

  while(window.isOpen())
  {
    while(std::optional event = window.pollEvent())
    {
      if(event->is<sf::Event::Closed>())
      {
        window.close();
      }

      if (const auto* key = event->getIf<sf::Event::KeyPressed>())
      {
        // key is already a pointer, we dont need to pass it down as an address
        handle_keys(key, MAP_SIZE, MARGIN, map, window);
      }

      handle_mouse(MAP_SIZE, MARGIN, map, window);
    }

    window.clear(sf::Color::White);
    map.draw(window);
    window.display();
  }

  return 0;
}

