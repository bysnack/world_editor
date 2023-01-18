#include "level/tile_map.hpp"

#include "level/map.hpp"

namespace level {

std::array<ecs::Entity, 4> make_tile_map(const graphics::Atlas &atlas) {
  Map map{"Maps/Mapa1"};
  const auto width = 100u;
  const auto height = 100u;
  std::array<sf::VertexArray, 4> layers;
  for (auto &layer : layers) {
    // resize the vertex array to fit the level size
    layer.setPrimitiveType(sf::Quads);
    layer.resize(width * height * 4);
  }

  const sf::Texture &tileset = atlas.texture();
  const auto &grhs = atlas.grhs();
  const auto &tiles = map.tiles();

  const auto tileSize = 32;

  // populate the vertex array, with one quad per tile
  for (unsigned int i = 0; i < width; ++i) {
    for (unsigned int j = 0; j < height; ++j) {
      // get the current tile number
      const auto tileNumber = j + i * width;

      for (auto layer = 0u; layer < layers.size(); ++layer) {
        // get a pointer to the current tile's quad
        sf::Vertex *quad = &layers[layer][tileNumber * 4];
        const auto grh = tiles.at(tileNumber).grh[layer];
        if (!grhs.contains(grh)) {
          continue;
        }
        const auto &grh_rect = grhs.at(grh);

        if (layer != 0) {
          const auto offset =
              sf::Vector2f(grh_rect.width / 2, grh_rect.height) -
              sf::Vector2f(tileSize / 2, tileSize);
          // define its 4 corners
          quad[0].position = sf::Vector2f(j * tileSize, i * tileSize) - offset;
          quad[1].position =
              sf::Vector2f((j * tileSize) + grh_rect.width, i * tileSize) -
              offset;
          quad[2].position = sf::Vector2f((j * tileSize) + grh_rect.width,
                                          (i * tileSize) + grh_rect.height) -
                             offset;
          quad[3].position =
              sf::Vector2f(j * tileSize, (i * tileSize) + grh_rect.height) -
              offset;
        } else {
          // define its 4 corners
          quad[0].position = sf::Vector2f(j * tileSize, i * tileSize);
          quad[1].position = sf::Vector2f((j + 1) * tileSize, i * tileSize);
          quad[2].position =
              sf::Vector2f((j + 1) * tileSize, (i + 1) * tileSize);
          quad[3].position = sf::Vector2f(j * tileSize, (i + 1) * tileSize);
        }

        // define its 4 texture coordinates
        quad[0].texCoords = sf::Vector2f(grh_rect.left, grh_rect.top);
        quad[1].texCoords =
            sf::Vector2f(grh_rect.left + grh_rect.width, grh_rect.top);
        quad[2].texCoords = sf::Vector2f(grh_rect.left + grh_rect.width,
                                         grh_rect.top + grh_rect.height);
        quad[3].texCoords =
            sf::Vector2f(grh_rect.left, grh_rect.top + grh_rect.height);
      }
    }
  }

  return {ecs::Entity{layers[0], tileset}, ecs::Entity{layers[1], tileset},
          ecs::Entity{layers[2], tileset}, ecs::Entity{layers[3], tileset}};
}
} // namespace level
