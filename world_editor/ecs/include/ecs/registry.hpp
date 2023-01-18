#pragma once

#include "ecs/injector.hpp"

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

namespace ecs {

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

  template <class Task> void register_task(Task task) {
    m_tasks.emplace_back(
        [task = std::move(task)](ecs::Injector &in) { in.run(task); });
  }

  template <class System> void register_system(System system) {
    m_systems.try_emplace(
        typeid(system),
        [system = std::move(system)](ecs::Injector &in) { in.run(system); });
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
  std::vector<Injector::Injectable> m_tasks;
  std::unordered_map<std::type_index, Injector::Injectable> m_systems;
};

} // namespace ecs
