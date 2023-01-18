#pragma once

#include "ecs/injector.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>

namespace events {

class Handler {
public:
  using Container =
      std::unordered_map<sf::Event::EventType,
                         std::function<void(sf::Event, ecs::Injector &)>>;
  void poll_event(sf::RenderWindow &target, ecs::Injector &injector) {
    sf::Event e;
    while (target.pollEvent(e)) {
      if (m_container.contains(e.type)) {
        m_container.at(e.type)(e, injector);
      }
    }
  }

  Container::mapped_type &operator[](sf::Event::EventType type) {
    return m_container[type];
  }

private:
  Container m_container;
};

} // namespace events