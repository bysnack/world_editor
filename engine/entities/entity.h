#pragma once
#include <unordered_map>
#include <typeindex>
#include "../components/components.h"
#include <type_traits>
#include <tuple>

namespace engine::entities {

    struct entity {
        entity() = default;

        template<class ...components_t>
        entity(components_t&& ...components) {
            add_components(std::forward<components_t>(components)...);
        }

        template<class ...component_t>
        void add_components(component_t&& ...component) {
            (
                add_component<component_t>(std::forward<component_t>(component))
            , ...);
        }

        template<class component_t, typename ...args_t>
        void add_component(args_t&& ...args) {
            _components.emplace(
                std::piecewise_construct_t{},
                std::forward_as_tuple(typeid(component_t)),
                std::forward_as_tuple(std::make_unique<component_t>(std::forward<args_t>(args)...))
            );
        }

        template<class component_t>
        component_t* get_component() {
            return static_cast<component_t*>(_components.at(typeid(component_t)).get());
        }

        template<class ...components_t>
        std::tuple<components_t*...> get_components() {
            return {
                get_component<components_t>()...
            };
        }

        template<class ...component_t>
        bool has_components() noexcept {
            // prepare type info container
            std::list<std::type_index> info{ typeid(component_t)... };

            // for each component
            for (auto&& [key, value] : _components) {
                // for each type
                for (auto&& type : info) {
                    // remove from type list if matches
                    if (key == type) {
                        info.remove(key);

                        // a match was found already, no point in continuing
                        break;
                    }
                }
                // are we out of types to check?
                if (info.empty()) {
                    return true;
                }
            }

            return false;
        }

        std::unordered_map<std::type_index, std::unique_ptr<components::component>> _components;
    };
}