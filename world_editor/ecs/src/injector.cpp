#include "ecs/injector.hpp"

#include "ecs/make.hpp"

#include <algorithm>

namespace ecs {

Injector::Injector(std::queue<Injectable> init_queue,
                   const std::vector<Injectable> &task_list,
                   std::unordered_map<std::type_index, Injectable> systems)
    : m_systems{std::move(systems)} {
  inject(Make<ecs::Entities>{});
  while (!init_queue.empty()) {
    std::invoke(init_queue.front(), *this);
    init_queue.pop();
  }

  for (const auto &task : task_list) {
    std::invoke(task, *this);
  }
}

void Injector::run_systems() {
  for (const auto &[_, system] : m_systems) {
    std::invoke(system, *this);
  }
}

} // namespace ecs
