#pragma once

#include "core/core.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace sfml {

struct Camera {
  enum class Heading { south, north, east, west };
  sf::View view;
  Heading heading = {};
  float speed = 1.f;
  bool is_moving = false;
};

struct Views {
  Camera application;
  Camera renderer;
  Camera selected;
};

struct EventHandler {
  bool poll_event(sf::RenderWindow &window) {
    sf::Event e;
    if (window.pollEvent(e)) {
      event.push(std::move(e));
      return true;
    }

    return false;
  }

  std::queue<sf::Event> event;
};

class Application {
public:
  Application();

  void run();

  template <class Make> void register_dependency(Make make) {
    m_engine.register_dependency(std::forward<Make>(make));
  }

  template <class System> void register_system(System system) {
    m_engine.register_system(std::forward<System>(system));
  }

  template <class Task> void register_task(Task task) {
    m_engine.register_task(std::forward<Task>(task));
  }

private:
  sf::Clock _clock;
  engine::Core m_engine;
};

} // namespace sfml