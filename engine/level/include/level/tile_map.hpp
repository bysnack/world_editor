#pragma once

#include "di/entity.hpp"
#include <SFML/Graphics.hpp>

namespace level {

di::Entity make_tile_map(const std::string &path, sf::Vector2u tileSize,
                         const int *tiles, unsigned int width,
                         unsigned int height);

} // namespace level
