#pragma once

#include "di/entity.hpp"
#include "meta/type_traits.hpp"

#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>

namespace di {

template <class... Components> class View {
public:
  View(const std::unordered_map<std::type_index, std::shared_ptr<void>>
           &dependencies) {
    for (auto &entity : *std::static_pointer_cast<Entities>(
             dependencies.at(typeid(Entities)))) {
      if (entity.has_components<Components...>()) {
        m_entities.push_back(std::ref(entity));
      }
    }
  }

  auto begin() { return m_entities.begin(); }
  auto end() { return m_entities.end(); }
  auto begin() const { return m_entities.begin(); }
  auto end() const { return m_entities.end(); }

private:
  std::vector<std::reference_wrapper<Entity>> m_entities;
};

} // namespace di
