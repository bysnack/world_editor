#include "main/engine.hpp"
#include <iostream>

struct player_move {
  void operator()(std::map<uint32_t, sf::VertexArray> &drawables) const {
    if (drawables.empty()) {
      return;
    }
    auto &triangle = drawables[0];
    float elapsed_time = 0.01;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      triangle[0].position = sf::Vector2f(10.f + (1000.f * elapsed_time), 10.f);
      triangle[1].position =
          sf::Vector2f(100.f + (1000.f * elapsed_time), 10.f);
      triangle[2].position =
          sf::Vector2f(100.f + (1000.f * elapsed_time), 100.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      triangle[0].position =
          sf::Vector2f(10.f + (-1000.f * elapsed_time), 10.f);
      triangle[1].position =
          sf::Vector2f(100.f + (-1000.f * elapsed_time), 10.f);
      triangle[2].position =
          sf::Vector2f(100.f + (-1000.f * elapsed_time), 100.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      triangle[0].position =
          sf::Vector2f(10.f, 10.f + (-1000.f * elapsed_time));
      triangle[1].position =
          sf::Vector2f(100.f, 10.f + (-1000.f * elapsed_time));
      triangle[2].position =
          sf::Vector2f(100.f, 100.f + (-1000.f * elapsed_time));
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      triangle[0].position = sf::Vector2f(10.f, 10.f + (1000.f * elapsed_time));
      triangle[1].position =
          sf::Vector2f(100.f, 10.f + (1000.f * elapsed_time));
      triangle[2].position =
          sf::Vector2f(100.f, 100.f + (1000.f * elapsed_time));
    }
  }
};

enum class foo { one, two, three };

template <foo f> struct Dep { std::string text; };

int main() {
  engine::core engine{};
  engine.register_dependency([] {
    auto triangle = [] {
      sf::VertexArray triangle(sf::Triangles, 3);
      // define the position of the triangle's points
      triangle[0].position = sf::Vector2f(10.f, 10.f);
      triangle[1].position = sf::Vector2f(100.f, 10.f);
      triangle[2].position = sf::Vector2f(100.f, 100.f);

      // define the color of the triangle's points
      triangle[0].color = sf::Color::Red;
      triangle[1].color = sf::Color::Blue;
      triangle[2].color = sf::Color::Green;

      return triangle;
    }();

    return di::Entities{di::Entity{triangle}};
  });

  engine.register_system(
      [](sf::RenderWindow &window, di::View<sf::VertexArray> drawables) {
        for (auto entity : drawables) {
          window.draw(entity.get().at<sf::VertexArray>());
        }
      });

  //   engine.register_system(player_move{});

  engine.run();
  //   engine::config config{"Source/INIT/", "Source/Graphics/",
  //   "Source/Fonts/",
  //                         1920, 1080};
  //   engine::core core{std::move(config)};
  //   core.systems.emplace<player_move>(core.entities);

  //   const auto &atlas = core.resources.get<engine::resources::atlas>();

  //   // lets make a tile map
  //   for (uint32_t j = 0; j < 100; ++j) {
  //     for (uint32_t i = 0; i < 100; ++i) {
  //       core.entities.create(
  //           "floor" + std::to_string(i + j * 100),
  //           engine::components::mesh{atlas[12439 + (i % 4) + ((j % 4) *
  //           4)],
  //                                    {32.f, 32.f},
  //                                    {0.f, 0.f},
  //                                    2},
  //           engine::components::position{{i * 32.f, j * 32.f}});
  //     }
  //   }

  //   core.entities.create(
  //       "player",
  //       engine::components::mesh{atlas[12469], {32.f, 32.f},
  //       {32.f, 32.f}, 4}, engine::components::position{{32.f, 32.f}},
  //       engine::components::size{{32.f, 32.f}},
  //       engine::components::speed{1000.f});

  //   core.run();

  return 0;
}