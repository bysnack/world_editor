#pragma once

#include "di/make.hpp"
#include "di/registry.hpp"

namespace engine {

class Core {
public:
  di::Injector &initialize() {
    m_registry.register_dependency(
        [&]() { return std::move(m_initial_entities); });

    m_injector = m_registry.build_injector();

    for (auto task : m_tasks) {
      std::invoke(task, *m_injector);
    }

    return *m_injector;
  }

  template <class Make> void register_dependency(Make make) {
    if (m_injector) {
      return;
    }

    m_registry.register_dependency(std::forward<Make>(make));
  }

  template <class... Components>
  void register_entity(Components... components) {
    if (m_injector) {
      m_injector->run([&](di::Entities &entities) {
        entities.emplace_back(std::move(components)...);
      });
    } else {
      m_initial_entities.emplace_back(std::move(components)...);
    }
  }

  template <class System> void register_system(System system) {
    m_systems.try_emplace(
        typeid(system),
        [system = std::move(system)](di::Injector &in) { in.run(system); });
  }

  template <class Task> void register_task(Task task) {
    m_tasks.emplace_back(
        [task = std::move(task)](di::Injector &in) { in.run(task); });
  }

  void run_systems() {
    if (!m_injector) {
      return;
    }

    for (auto [_, system] : m_systems) {
      std::invoke(system, *m_injector);
    }
  }

private:
  di::Registry m_registry;
  std::optional<di::Injector> m_injector;
  std::unordered_map<std::type_index, std::function<void(di::Injector &)>>
      m_systems;
  di::Entities m_initial_entities;
  std::vector<std::function<void(di::Injector &)>> m_tasks;
};

} // namespace engine