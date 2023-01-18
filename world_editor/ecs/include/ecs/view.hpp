#pragma once

#include "ecs/entity.hpp"
#include "ecs/type_traits.hpp"

#include <iostream>
#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace ecs {

template <class... Components> class View {
public:
  View(const std::unordered_map<std::type_index, std::shared_ptr<void>>
           &dependencies) {
    for (auto &entity : *std::static_pointer_cast<Entities>(
             dependencies.at(typeid(Entities)))) {
      if (entity.has_components<Components...>()) {
        m_entities.push_back(entity.tie<Components...>());
      }
    }
  }

  auto begin() { return m_entities.begin(); }
  auto end() { return m_entities.end(); }
  auto begin() const { return m_entities.begin(); }
  auto end() const { return m_entities.end(); }

  bool empty() const { return m_entities.empty(); }

private:
  std::vector<std::tuple<std::decay_t<Components> &...>> m_entities;
};

} // namespace ecs
