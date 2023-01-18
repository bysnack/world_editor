#pragma once

#include <array>
#include <fstream>

namespace camera {

enum class ID { application, renderer, selected };
enum class Heading { south, north, east, west };
enum class Status {
  is_moving = 0x01,
};

} // namespace camera