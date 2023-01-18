#include "ecs/registry.hpp"

#include <algorithm>

namespace ecs {

namespace {

/// Check through the graph to see if anything has `item` as a dependent
bool is_a_dependent(std::type_index item, const Registry::Graph &graph) {
  return std::any_of(graph.begin(), graph.end(), [item](const auto &pair) {
    const auto &dependents = pair.second.dependents;
    return std::find(dependents.begin(), dependents.end(), item) !=
           dependents.end();
  });
};

} // namespace

Injector Registry::build_injector() {
  auto remaining_graph = m_graph;
  auto result = std::queue<std::function<void(Injector &)>>{};
  auto not_dependents = std::deque<std::pair<std::type_index, int>>{};

  // Find all the nodes that aren't initially a dependent
  for (const auto &[key, node] : remaining_graph) {
    if (!is_a_dependent(key, remaining_graph)) {
      not_dependents.emplace_back(key, 0);
    }
  }

  while (!not_dependents.empty()) {
    const auto [target_type, rank] = not_dependents.front();
    not_dependents.pop_front();
    result.push(m_graph.find(target_type)->second.register_dependency);
    remaining_graph.erase(target_type);

    for (const auto &[type, node] : remaining_graph) {
      if (!is_a_dependent(type, remaining_graph) &&
          std::find_if(not_dependents.begin(), not_dependents.end(),
                       [type = type](const auto &elem) {
                         return elem.first == type;
                       }) == not_dependents.end()) {
        not_dependents.emplace_back(type, rank + 1);
      }
    }
  }

  if (!remaining_graph.empty()) {
    throw std::runtime_error("Cycling dependency found");
  }

  return Injector{std::move(result), std::move(m_tasks), std::move(m_systems)};
}

} // namespace ecs
