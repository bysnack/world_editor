#include "di/injector.hpp"

#include <algorithm>

namespace di {

Injector::Injector(std::queue<std::function<void(Injector &)>> init_queue) {
  while (!init_queue.empty()) {
    std::invoke(init_queue.front(), *this);
    init_queue.pop();
  }
}

} // namespace di
