#include "camera/init.hpp"

#include "camera/components.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>

namespace camera {
namespace {
struct View {
  ID id;
  Heading heading = Heading::south;
  float speed = 1.f;
  bool is_moving = false;

  void operator()(ecs::Entities &entities,
                  const std::unique_ptr<sf::RenderWindow> &window) const {
    const auto window_size = sf::Vector2f{window->getSize()};
    entities.emplace_back(
        id, sf::View{sf::FloatRect{0.f, 0.f, window_size.x, window_size.y}},
        heading, speed, is_moving);
  }
};

// void make_base_views(const std::unique_ptr<sf::RenderWindow> &window,
//                      ecs::Entities &entities) {
//   const auto window_size = sf::Vector2f{window->getSize()};
//   entities.push_back(
//       ecs::Entity{ID::renderer,
//                  sf::View{sf::FloatRect{0.f, 0.f, window_size.x,
//                  window_size.y},
//                           Heading::south, 1.f, false}});
//   entities.push_back(
//       ecs::Entity{ID::renderer,
//                  sf::View{sf::FloatRect{0.f, 0.f, window_size.x,
//                  window_size.y},
//                           Heading::south, 1.f, false}});
//   // entities.push_back(
//   //     ecs::Entity{ID::renderer,
//   //                sf::View{sf::Vector2f{16.f, 16.f},
//   // sf::Vector2f{32.f, 32.f}},
//   //                Heading::south, 1.f, false});

//   // views.renderer.view.setViewport({.16f, .16f, .83f, .70f});
//   // views.selected.view.setViewport({0.01, 0.01f, 0.14, 0.14f});
//   return views;
// }

void renderer_movement_system(
    ecs::View<ID, sf::View, Heading, float, bool> cameras,
    const std::chrono::milliseconds &elapsed_time) {

  for (auto &[id, view, heading, speed, is_moving] : cameras) {
    if (!is_moving) {
      return;
    }

    switch (heading) {
    case Heading::north:
      view.move(0.f, -speed * elapsed_time.count());
      break;
    case Heading::south:
      view.move(0.f, speed * elapsed_time.count());
      break;
    case Heading::east:
      view.move(-speed * elapsed_time.count(), 0.f);
      break;
    case Heading::west:
      view.move(speed * elapsed_time.count(), 0.f);
      break;
    default:
      break;
    }
  }
}
} // namespace

void register_module(ecs::Registry &registry) {
  registry.register_task(View{ID::application});
  registry.register_task(View{ID::renderer});
  registry.register_system(&renderer_movement_system);
}

} // namespace camera
