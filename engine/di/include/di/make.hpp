#pragma once

#include "di/entity.hpp"
#include "meta/type_traits.hpp"

#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>

namespace di {

template <class Target, class... Args> struct Make {
  Make(Args &&...args) : deps{std::forward<Args>(args)...} {}

  std::tuple<Args...> deps;

  Target operator()() const {
    return std::apply([](auto &&...d) { return Target{d...}; }, deps);
  }
};

} // namespace di
