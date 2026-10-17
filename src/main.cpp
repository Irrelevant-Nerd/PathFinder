#include <SFML/Graphics.hpp>
#include <iostream>
#include <map.h>
#include <path.h>
#include <vector>
#include <chrono>

void handle_keys(const auto* key, int MAP_SIZE, int MARGIN, Map& map, sf::RenderWindow& window, std::vector<sf::Vector3i>& path, Path& path_obj)
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

  // track time to know which algorithm is faster BFS or Dijkstras algorithm
  if (key->code == sf::Keyboard::Key::T)
  {
    const int runs = 100;
    std::vector<sf::Vector3i> p;

    auto t0 = std::chrono::steady_clock::now();

    for (int r = 0; r < runs; ++r)
    {
      p.clear();
      p.emplace_back(map.get_end().x, map.get_end().y, 0);
      path_obj.breadth_first_search(p, map);
      path_obj.clear_paths(p, map);
    }
    auto t1 = std::chrono::steady_clock::now();

    for (int r = 0; r < runs; ++r)
      path_obj.dijkstras_algorithm(p, map);
    auto t2 = std::chrono::steady_clock::now();

    using us = std::chrono::duration<double, std::micro>;
    std::cout << "BFS:      " << us(t1 - t0).count() / runs << " us\n";
    std::cout << "Dijkstra: " << us(t2 - t1).count() / runs << " us\n";
  }

  if (key->code == sf::Keyboard::Key::D)
  {
    if (path_obj.dijkstras_algorithm(path, map))
    {
      map.add_path(path);
      std::cout << "Dijkstras algorithm has found a path. Finished\n";
    }
    else
    {
      std::cout << "No path has found. Press 'R' to generate map again\n";
      path.clear();
    }
  }

  // start a path
  if (key->code == sf::Keyboard::Key::B)
  {
    // make sure the path is empty before seaching again
    // but the old path will remain instead of creating one again if the end remains unchanged
    if(!path.empty()) path.clear();

    path.emplace_back(map.get_end().x, map.get_end().y, 0);

    if(path_obj.breadth_first_search(path, map))
    {
      path_obj.clear_paths(path, map);
      map.add_path(path);
      std::cout << "Breadth-first search has found a path. Finished\n";
    }
    else
    {
      std::cout << "No path has found. Press 'R' to generate map again\n";
      path.clear();
    }
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
}
// handles the mouse
void handle_mouse(int MAP_SIZE, int MARGIN, Map &map, sf::RenderWindow &window)
{
  // puts a wall on a cell by left clicking
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

  // removes a wall on a cell by right clicking
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
  constexpr int wWIDTH = 900;
  constexpr int wHEIGHT = 900;

  int MAP_SIZE = 50;
  int MARGIN = 50;

  sf::RenderWindow window(sf::VideoMode({wWIDTH, wHEIGHT}), "Path Finder");

  Map map((1280 - MARGIN * 2) / MAP_SIZE, MAP_SIZE, MARGIN);
  Path path_obj;

  sf::Font font("assets/fonts/HomeVideo-BLG6G.ttf");
  sf::Text text(font);
  text.setString("'S' = Start position, 'E' = End position, 'R' = Random map, 'B' = BFS, 'D' = dijkstras_algorithm, 'C' = Clear map.");
  text.setFillColor(sf::Color::Black);
  text.setCharacterSize(13);
  text.setPosition({0, wHEIGHT - (wHEIGHT - 20)});

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
        handle_keys(key, MAP_SIZE, MARGIN, map, window, path_obj.get_path(), path_obj);
      }

      handle_mouse(MAP_SIZE, MARGIN, map, window);
    }

    window.clear(sf::Color::White);
    map.draw(window);
    window.draw(text);
    window.display();
  }

  return 0;
}

