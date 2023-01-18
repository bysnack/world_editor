#pragma once

#include <array>
#include <fstream>

namespace level {
template <class T, size_t size = sizeof(T)> T read(std::ifstream &in) {
  T retval;
  in.read(reinterpret_cast<char *>(&retval), size);

  return retval;
}

struct Obj {
  uint16_t index;
  uint16_t amount;
};

struct WorldPos {
  uint16_t map;
  uint16_t x;
  uint16_t y;
};

struct MapBlock {
  bool blocked;
  std::array<uint32_t, 4> grh;
  uint16_t trigger;
  uint32_t particle_group_index;
  WorldPos tile_exit;
  uint16_t npc_index;
  Obj object_info;
};

struct Header {
  std::array<char, 255> description;
  uint32_t crc;
  uint32_t magic_word;

  Header(std::ifstream &in)
      : description{read<std::array<char, 255>, 255>(in)},
        crc{read<uint32_t>(in)}, magic_word{read<uint32_t>(in)} {}

  Header() = default;
};
} // namespace level