#pragma once

#include "level/data.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

namespace level {
/// Initializes a map from it's `.map` and `.inf` files
class Map {
public:
  static constexpr auto map_tile_width = 100;
  static constexpr auto map_tile_height = 100;

  Map(const std::string &path) {
    std::ifstream map_file{path + ".map", std::ios::binary};
    std::ifstream inf_file{path + ".inf", std::ios::binary};

    // Load version and header
    auto version = read<uint16_t>(map_file);
    auto header = Header{map_file};
    read<uint16_t>(map_file);
    read<uint16_t>(map_file);
    read<uint16_t>(map_file);
    read<uint16_t>(map_file);

    read<uint16_t>(inf_file);
    read<uint16_t>(inf_file);
    read<uint16_t>(inf_file);
    read<uint16_t>(inf_file);
    read<uint16_t>(inf_file);

    m_tiles.resize(map_tile_width * map_tile_height);
    for (auto y = 0u; y < 100; ++y) {
      for (auto x = 0u; x < 100; ++x) {
        // Load tile flags
        const auto block_index = x + (y * 100);
        auto flags = read<uint8_t>(map_file);

        m_tiles[block_index].blocked = flags & 1;

        // Read first layer
        m_tiles[block_index].grh[0] = read<uint32_t>(map_file);

        // Read second layer, only if it's used
        if (flags & 2) {
          m_tiles[block_index].grh[1] = read<uint32_t>(map_file);
        }

        if (flags & 4) {
          m_tiles[block_index].grh[2] = read<uint32_t>(map_file);
        }

        if (flags & 8) {
          m_tiles[block_index].grh[3] = read<uint32_t>(map_file);
        }

        if (flags & 16) {
          m_tiles[block_index].trigger = read<uint16_t>(map_file);
        }

        if (flags & 32) {
          m_tiles[block_index].particle_group_index = read<uint32_t>(map_file);
        }

        flags = read<uint8_t>(inf_file);

        if (flags & 1) {
          m_tiles[block_index].tile_exit = {read<uint16_t>(inf_file),
                                            read<uint16_t>(inf_file),
                                            read<uint16_t>(inf_file)};
        }

        if (flags & 2) {
          m_tiles[block_index].npc_index = read<uint16_t>(inf_file);
        }

        if (flags & 4) {
          m_tiles[block_index].object_info = {read<uint16_t>(inf_file),
                                              read<uint16_t>(inf_file)};
        }
      }
    }
  }

  const std::vector<MapBlock> &tiles() const { return m_tiles; }

private:
  std::vector<MapBlock> m_tiles;
  Header m_header;
};
} // namespace level