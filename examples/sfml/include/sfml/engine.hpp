#pragma once

#include "SFML/Graphics.hpp"
#include "core/core.hpp"

namespace sfml {

class Example {
public:
  Example() {
    register_dependency([]() {
      return sf::View{{960.f, 540.f}, {1920.f, 1080.f}};
    });
    register_dependency([](sf::View &camera) {
      return sf::RenderWindow{{800, 600}, "Argentum Online C++ Basic Engine"};
    });
    m_engine.register_task([](sf::RenderWindow &window, sf::View &view) {
      window.setView(view);
      window.setFramerateLimit(0);
      window.setVerticalSyncEnabled(true);
    });
  }

  void run() {
    auto &injector = m_engine.initialize();

    auto &target = injector.resolve<sf::RenderWindow &>();

    float elapsed_time = 0.f;

    // start the game loop
    while (target.isOpen()) {
      // listen for events
      sf::Event event;
      while (target.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          target.close();
          return;
        }
      }

      target.clear(sf::Color::Black);

      m_engine.run_systems();

      // run on user update systems
      target.display();
    }
  }

  template <class Make> void register_dependency(Make make) {
    m_engine.register_dependency(make);
  }

  template <class System> void register_system(System system) {
    m_engine.register_system(system);
  }

private:
  sf::Clock _clock;
  engine::Core m_engine;
};

} // namespace sfml