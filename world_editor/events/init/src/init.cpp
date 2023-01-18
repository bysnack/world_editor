#include "events/init.hpp"

#include "camera/components.hpp"
#include "events/handler.hpp"

namespace events {
namespace {
Handler make_event_handler() {
  auto handler = Handler{};
  handler[sf::Event::Closed] = [](sf::Event e, ecs::Injector &in) {
    in.resolve<const std::unique_ptr<sf::RenderWindow> &>()->close();
  };

  handler[sf::Event::KeyPressed] = [](sf::Event event, ecs::Injector &in) {
    auto &target = in.resolve<const std::unique_ptr<sf::RenderWindow> &>();
    if (event.key.code == sf::Keyboard::Escape) {
      target->close();
    }

    in.run([&](ecs::View<camera::Heading, bool> views) {
      for (auto &[heading, is_moving] : views) {
        if (event.key.code == sf::Keyboard::Up) {
          heading = camera::Heading::north;
          is_moving = true;
        } else if (event.key.code == sf::Keyboard::Down) {
          heading = camera::Heading::south;
          is_moving = true;
        } else if (event.key.code == sf::Keyboard::Left) {
          heading = camera::Heading::east;
          is_moving = true;
        } else if (event.key.code == sf::Keyboard::Right) {
          heading = camera::Heading::west;
          is_moving = true;
        }
      }
    });
  };

  handler[sf::Event::KeyReleased] = [](sf::Event event, ecs::Injector &in) {
    in.run([&](ecs::View<camera::Heading, bool> views) {
      for (auto &[heading, is_moving] : views) {
        if (event.key.code == sf::Keyboard::Up &&
            heading == camera::Heading::north) {
          is_moving = false;
        } else if (event.key.code == sf::Keyboard::Down &&
                   heading == camera::Heading::south) {
          is_moving = false;
        } else if (event.key.code == sf::Keyboard::Left &&
                   heading == camera::Heading::east) {
          is_moving = false;
        } else if (event.key.code == sf::Keyboard::Right &&
                   heading == camera::Heading::west) {
          is_moving = false;
        }
      }
    });
  };

  return handler;
}
} // namespace

void register_module(ecs::Registry &registry) {
  registry.register_dependency(&make_event_handler);
}

} // namespace events
