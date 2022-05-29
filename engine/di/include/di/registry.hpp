#pragma once

#include "di/injector.hpp"

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

namespace di {

class Registry {
public:
  struct Node {
    std::function<void(Injector &)> register_dependency;
    std::unordered_set<std::type_index> dependents;
  };

  using Graph = std::unordered_map<std::type_index, Node>;

  template <class Make> void register_dependency(Make make) {
    using Signature = meta::function_traits_t<Make>;
    using Return = std::decay_t<meta::at_t<0, Signature>>;

    register_dependency_impl(std::forward<Make>(make), Signature{});
  }

  Injector build_injector();

private:
  template <class Make, class Res, class... Deps>
  void register_dependency_impl(Make make, meta::ts<Res, Deps...>) {
    m_graph[typeid(Res)].register_dependency =
        [make = std::forward<Make>(make)](auto &in) { in.inject(make); };

    (m_graph[typeid(std::decay_t<Deps>)].dependents.insert(
         typeid(std::decay_t<Res>)),
     ...);
  }

  Graph m_graph;
};

} // namespace di
