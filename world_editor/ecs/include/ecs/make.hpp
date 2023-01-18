#pragma once

#include "ecs/entity.hpp"
#include "ecs/type_traits.hpp"

#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>

namespace ecs {

template <class Target, class... Args> struct Make {
  Make(Args &&...args) : deps{std::forward<Args>(args)...} {}

  std::tuple<Args...> deps;

  Target operator()() const {
    return std::apply([](auto &&...d) { return Target{d...}; }, deps);
  }
};

} // namespace ecs
