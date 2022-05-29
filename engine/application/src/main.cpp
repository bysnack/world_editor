#include "application/application.hpp"
#include "graphics/atlas.hpp"
#include "level/tile_map.hpp"

#include <iostream>
#include <random>

int main() {
  sfml::Application example{};
  example.register_dependency([] { return di::Entities{}; });
  example.register_dependency([] {
    return graphics::Atlas{"graphics.json", "resources/graphics"};
  });
  example.register_task([](tgui::GuiSFML &gui, const graphics::Atlas &atlas) {
    gui.loadWidgetsFromFile("main.txt");
    if (auto listBox = gui.get<tgui::ListBox>("entities_list_box")) {
      for (const auto &[id, rect] : atlas.grhs()) {
        listBox->addItem(std::to_string(id));
      }
    }
  });

  example.register_system([](sfml::Views &views, tgui::GuiSFML &gui,
                             const std::unique_ptr<sf::RenderWindow> &target,
                             const graphics::Atlas &atlas) {
    if (auto listBox = gui.get<tgui::ListBox>("entities_list_box")) {
      if (auto index = listBox->getSelectedItem(); !index.empty()) {
        auto grh = std::stoi(index.toStdString());
        const auto &tile = atlas.grhs().at(grh);
        sf::Sprite sprite{atlas.texture(), tile};
        const auto size = sf::Vector2f{static_cast<float>(tile.width),
                                       static_cast<float>(tile.height)};
        views.selected.view.setSize(size);
        views.selected.view.setCenter(size / 2.f);
        target->setView(views.selected.view);
        target->draw(sprite);
      }
    }
  });
  example.register_system([](sfml::EventHandler &handler, sfml::Views &views,
                             const std::unique_ptr<sf::RenderWindow> &target) {
    if (handler.event.empty()) {
      return;
    }

    const auto event = handler.event.front();

    switch (event.type) {
    case sf::Event::Closed:
      target->close();
      break;
    case sf::Event::KeyPressed:
      if (event.key.code == sf::Keyboard::Escape) {
        target->close();
      } else if (event.key.code == sf::Keyboard::Up) {
        views.renderer.heading = sfml::Camera::Heading::north;
        views.renderer.is_moving = true;
      } else if (event.key.code == sf::Keyboard::Down) {
        views.renderer.heading = sfml::Camera::Heading::south;
        views.renderer.is_moving = true;
      } else if (event.key.code == sf::Keyboard::Left) {
        views.renderer.heading = sfml::Camera::Heading::east;
        views.renderer.is_moving = true;
      } else if (event.key.code == sf::Keyboard::Right) {
        views.renderer.heading = sfml::Camera::Heading::west;
        views.renderer.is_moving = true;
      }
      break;
    case sf::Event::KeyReleased:
      if (event.key.code == sf::Keyboard::Up &&
          views.renderer.heading == sfml::Camera::Heading::north) {
        views.renderer.is_moving = false;
      } else if (event.key.code == sf::Keyboard::Down &&
                 views.renderer.heading == sfml::Camera::Heading::south) {
        views.renderer.is_moving = false;
      } else if (event.key.code == sf::Keyboard::Left &&
                 views.renderer.heading == sfml::Camera::Heading::east) {
        views.renderer.is_moving = false;
      } else if (event.key.code == sf::Keyboard::Right &&
                 views.renderer.heading == sfml::Camera::Heading::west) {
        views.renderer.is_moving = false;
      }
      break;
    default:
      break;
    }

    handler.event.pop();
  });
  example.register_system([](sfml::Views &views,
                             const std::chrono::milliseconds &elapsed_time) {
    if (!views.renderer.is_moving) {
      return;
    }

    switch (views.renderer.heading) {
    case sfml::Camera::Heading::north:
      views.renderer.view.move(0.f,
                               -views.renderer.speed * elapsed_time.count());
      break;
    case sfml::Camera::Heading::south:
      views.renderer.view.move(0.f,
                               views.renderer.speed * elapsed_time.count());
      break;
    case sfml::Camera::Heading::east:
      views.renderer.view.move(-views.renderer.speed * elapsed_time.count(),
                               0.f);
      break;
    case sfml::Camera::Heading::west:
      views.renderer.view.move(views.renderer.speed * elapsed_time.count(),
                               0.f);
      break;
    default:
      break;
    }
  });
  example.register_task([](di::Entities &entities) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine{rnd_device()}; // Generates random integers
    std::uniform_int_distribution<int> dist{0, 63};
    auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

    std::vector<int> level(10000);
    std::generate(level.begin(), level.end(), gen);

    entities.push_back(level::make_tile_map("tileset.png", sf::Vector2u{32, 32},
                                            level.data(), 100, 100));
  });

  example.register_system([](const std::unique_ptr<sf::RenderWindow> &window,
                             sfml::Views &views,
                             di::View<sf::VertexArray, sf::Texture> drawables) {
    window->setView(views.renderer.view);
    for (auto entity : drawables) {
      auto texture = entity.get().at<sf::Texture>();
      window->draw(entity.get().at<sf::VertexArray>(), &texture);
    }
  });

  example.run();

  return 0;
}