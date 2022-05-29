#pragma once

#include "di/entity.hpp"
#include "di/view.hpp"
#include "meta/type_traits.hpp"

#include <functional>
#include <memory>
#include <queue>
#include <typeindex>

namespace di {

class Injector {
public:
  Injector(std::queue<std::function<void(Injector &)>> init_queue);

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
    return *std::static_pointer_cast<std::decay_t<Dependency>>(
        m_dependencies.at(typeid(std::decay_t<Dependency>)));
  }

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
};

} // namespace di
