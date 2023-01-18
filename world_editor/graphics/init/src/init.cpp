#include "graphics/init.hpp"

#include "camera/components.hpp"
#include "graphics/atlas.hpp"

namespace graphics {
namespace {
std::unique_ptr<sf::RenderWindow> make_window() {
  auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode{1024, 768},
                                                   "Engine 2", sf::Style::None);
  window->setPosition(sf::Vector2i{0, 0});
  window->setFramerateLimit(0);
  window->setVerticalSyncEnabled(true);
  auto desktop = sf::VideoMode::getDesktopMode();
  window->setPosition(
      {static_cast<int>(desktop.width / 2 - window->getSize().x / 2),
       static_cast<int>(desktop.height / 2 - window->getSize().y / 2)});
  return window;
}

void rendering_system(const std::unique_ptr<sf::RenderWindow> &window,
                      ecs::View<sf::View, camera::ID> views,
                      ecs::View<sf::VertexArray, sf::Texture> drawables) {
  auto it = std::find_if(views.begin(), views.end(), [](const auto &v) {
    return std::get<camera::ID &>(v) == camera::ID::renderer;
  });

  if (it == views.end()) {
    return;
  }

  window->setView(std::get<sf::View &>(*it));
  for (auto &[vertex_array, texture] : drawables) {
    window->draw(vertex_array, &texture);
  }
}
} // namespace

void register_module(ecs::Registry &registry) {
  registry.register_dependency(&make_window);
  registry.register_dependency([] {
    return graphics::Atlas{"graphics.json", "resources/graphics"};
  });
  registry.register_system(&rendering_system);
}
} // namespace graphics
