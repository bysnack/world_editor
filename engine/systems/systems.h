#pragma once
#include <array>
#include "./run_condition.h"
#include "../entities/entities.h"
#include "./render.h"

namespace engine::systems {

    template<typename T>
    struct system_traits : public system_traits<decltype(&T::operator())> {};

    template <typename ret_t, typename class_t, typename... args_t>
    struct system_traits<ret_t (class_t::*)(args_t...)> {

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
    struct system_traits<ret_t (*)(args_t...)> {

        using return_type = ret_t;
        using arguments = std::tuple<args_t...>;
        using system = std::function<ret_t(args_t...)>;

        template <size_t N>
        struct argument {
            using type = std::tuple_element_t<N, arguments>;
        };
    };

    class manager {
    public:
        using on_update_system = std::function<void(entities::manager&, float)>;
        using on_start_system = std::function<void(entities::manager&)>;
        using on_event_system = std::function<void(entities::manager&, sf::Event&, float)>;
        template<typename ...systems_t>
        using container_type = std::tuple<std::vector<systems_t>...>;
        using system_container = container_type<on_update_system, on_start_system, on_event_system>;


        manager() = default;

        template<typename ...systems_t>
        manager(systems_t&& ...systems) {
            emplace(std::forward<systems_t>(systems)...);
        }

        template<typename ...systems_t>
        void emplace(systems_t&& ...systems) {
            (std::get<std::vector<typename system_traits<systems_t>::system>>(_systems).push_back(std::forward<systems_t>(systems)), ...);
        }

        template<typename ...args_t>
        void run(args_t&& ...arguments) {
            // for each system system of this condition
            for (auto&& system : std::get<std::vector<std::function<void(args_t...)>>>(_systems)) {
                // run with entities and elapsed time
                system(std::forward<args_t>(arguments)...);
            }
        }

    private:
        system_container _systems;
        //std::unordered_map<run_condition, std::vector<system_type>> _systems;
    };
}
