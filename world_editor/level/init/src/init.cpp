#include "level/init.hpp"

#include "level/tile_map.hpp"

namespace level {
namespace {
void make_map_entities(ecs::Entities &entities, const graphics::Atlas &atlas) {
  for (auto layer : level::make_tile_map(atlas)) {
    entities.push_back(layer);
  }
}
} // namespace

void register_module(ecs::Registry &registry) {
  registry.register_task(&make_map_entities);
}

} // namespace level
