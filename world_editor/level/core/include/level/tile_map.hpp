#pragma once

#include "ecs/entity.hpp"
#include "graphics/atlas.hpp"
#include <SFML/Graphics.hpp>

namespace level {

std::array<ecs::Entity, 4> make_tile_map(const graphics::Atlas &atlas);

} // namespace level
