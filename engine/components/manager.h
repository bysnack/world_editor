#pragma once
#include "./render.h"
#include "./component.h"
#include <memory>
#include <typeinfo>

namespace engine::components {
    class manager {
    public:
        template<class ...components_t>
        manager(components_t&& ...list)
        {
            (add(std::forward<components_t>(list)), ...);
        }

        template<class components_t, int size = std::tuple_size_v<components_t>>
        bool active() {
            if constexpr (size == 0) {
                return true;
            } else {
                return _components[typeid(std::tuple_element_t<size - 1, components_t>)] && active<components_t, size - 1>();
            }
        }

        template<class component_t>
        void add(component_t&& comp) {
            _components.emplace(
                std::piecewise_construct_t{},
                std::forward_as_tuple(typeid(component_t)),
                std::forward_as_tuple(std::make_shared<component_t>(std::forward<component_t>(comp)))
            );
        }

        template<class component_t>
        auto get() {
            return dynamic_cast<component_t*>(_components[typeid(component_t)].get());
        }
    private:
        std::unordered_map<std::type_index, std::shared_ptr<component>> _components;
    };
}
