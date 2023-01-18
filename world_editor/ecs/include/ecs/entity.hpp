#pragma once

#include "ecs/type_traits.hpp"

#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <typeindex>

namespace ecs {

class Entity {
public:
  template <class... Components> Entity(Components... components) {
    ((m_components[typeid(Components)] =
          std::make_unique<Components>(std::move(components))),
     ...);
  }

  template <class... Components> bool has_components() const {
    return ((m_components.contains(typeid(Components)) &&
             m_components.at(typeid(Components))) &&
            ... && true);
  }

  template <class System> void invoke(System system) {
    invoke_impl(system, meta::function_traits_t<System>{});
  }

  template <class Component> decltype(auto) at() {
    return *std::static_pointer_cast<Component>(
        m_components.at(typeid(Component)));
  }

  template <class... Components>
  std::tuple<std::decay_t<Components> &...> tie() {
    return {at<Components>()...};
  }

private:
  template <class System, class Ret, class... Components>
  void invoke_impl(System system, meta::ts<Ret, Components...>) {
    const auto should_execute = [&] {
      return (m_components[typeid(Components)] && ... && true);
    }();
    if (should_execute) {
      std::invoke(system, *std::static_pointer_cast<Components>(
                              m_components.at(typeid(Components)))...);
    }
  }

  std::map<std::type_index, std::shared_ptr<void>> m_components;
};

using Entities = std::vector<Entity>;

} // namespace ecs
