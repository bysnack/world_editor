#pragma once
#include <unordered_map>
#include <typeindex>
#include "../components/components.h"
#include <type_traits>
#include <tuple>

namespace engine::entities {


    template<typename T>
    struct function_traits : public function_traits<decltype(&T::operator())> {};

    template <typename ret_t, typename class_t, typename... args_t>
    struct function_traits<ret_t (class_t::*)(args_t...)> {

        using return_type = ret_t;
        using class_type = class_t;
        using arguments = std::tuple<args_t...>;
        using system = std::function<ret_t(args_t...)>;

        template <size_t N>
        struct argument {
            using type = std::tuple_element_t<N, arguments>;
        };
    };

    template <typename ret_t, typename... args_t>
    struct function_traits<ret_t (*)(args_t...)> {

        using return_type = ret_t;
        using arguments = std::tuple<args_t...>;
        using system = std::function<ret_t(args_t...)>;

        template <size_t N>
        struct argument {
            using type = std::tuple_element_t<N, arguments>;
        };
    };

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
            return dynamic_cast<component_t*>(_components.at(typeid(component_t)).get());
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

        template<typename handler_t>
        void with_components(handler_t&& handler) {
            using args_t = typename function_traits<handler_t>::arguments;
            args_t args;
            std::apply([&](auto&& ...arguments){
                handler(dynamic_cast<decltype(arguments)>(_components.at(typeid(decltype(arguments))).get())...);
            }, args);
        }

        std::unordered_map<std::type_index, std::unique_ptr<components::component>> _components;
    };
}