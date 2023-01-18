#pragma once

#include "ecs/entity.hpp"
#include "ecs/type_traits.hpp"
#include "ecs/view.hpp"

#include <functional>
#include <memory>
#include <queue>
#include <typeindex>

namespace ecs {

class Injector {
public:
  using Injectable = std::function<void(ecs::Injector &)>;

  Injector(std::queue<Injectable> init_queue,
           const std::vector<Injectable> &task_list,
           std::unordered_map<std::type_index, Injectable> systems);

  template <class System> void run(System system) {
    invoke_impl(system, meta::function_traits_t<System>{});
  }

  template <class System> void inject(System system) {
    using Signature = meta::function_traits_t<System>;
    using Return = meta::at_t<0, Signature>;
    m_dependencies[typeid(Return)].reset(
        new Return(invoke_impl(std::forward<System>(system), Signature{})));
  }

  template <class Dependency> Dependency resolve() {
    if constexpr (meta::is_v<View, Dependency>) {
      return Dependency{m_dependencies};
    } else {
      return *std::static_pointer_cast<std::decay_t<Dependency>>(
          m_dependencies.at(typeid(std::decay_t<Dependency>)));
    }
  }

  void run_systems();

private:
  template <class System, class Return, class... Deps>
  auto invoke_impl(System system, meta::ts<Return, Deps...>) {
    return std::invoke(system, cast<std::decay_t<Deps>>()...);
  }

  template <class Dep> decltype(auto) cast() {
    if constexpr (meta::is_v<View, Dep>) {
      return Dep{m_dependencies};
    } else if constexpr (std::is_same_v<Dep, Injector>) {
      return *this;
    } else {
      return *std::static_pointer_cast<Dep>(m_dependencies.at(typeid(Dep)));
    }
  }

  std::unordered_map<std::type_index, std::shared_ptr<void>> m_dependencies;
  std::unordered_map<std::type_index, Injectable> m_systems;
};

} // namespace ecs
