#pragma once

#include "camera/components.hpp"
#include "camera/init.hpp"
#include "ecs/registry.hpp"
#include "events/handler.hpp"
#include "events/init.hpp"
#include "graphics/init.hpp"
#include "level/init.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>

namespace engine {

void start() {
  ecs::Registry registry;

  registry.register_dependency([]() { return std::chrono::milliseconds{}; });
  graphics::register_module(registry);
  level::register_module(registry);
  camera::register_module(registry);
  events::register_module(registry);

  auto injector = registry.build_injector();

  auto &target = injector.resolve<const std::unique_ptr<sf::RenderWindow> &>();
  auto &event_handler = injector.resolve<events::Handler &>();
  auto &elapsed_time = injector.resolve<std::chrono::milliseconds &>();
  auto application_camera = [&] {
    auto views = injector.resolve<ecs::View<sf::View, camera::ID>>();
    return *std::find_if(views.begin(), views.end(), [](const auto &v) {
      return std::get<camera::ID &>(v) == camera::ID::application;
    });
  }();

  // start the game loop
  while (target->isOpen()) {
    auto start = std::chrono::steady_clock::now();
    // listen for events
    event_handler.poll_event(*target, injector);

    target->setView(std::get<sf::View &>(application_camera));
    target->clear(sf::Color::Black);
    injector.run_systems();
    target->display();
    elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
  }
}

} // namespace engine