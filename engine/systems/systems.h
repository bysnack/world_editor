#pragma once
#include <array>
#include <typeindex>
#include "../resources/resources.h"
#include "./run_condition.h"
#include "../entities/entities.h"
#include "./render.h"

namespace util {

    /**
     *  Unspecialized function traits
     */
    template <typename functor>
    struct function_traits
    {
        using nested_traits = function_traits<decltype(&functor::operator())>;
        using member_type   = typename nested_traits::member_type;
        using return_type   = typename nested_traits::return_type;
        using argument_type = typename nested_traits::argument_type;
        constexpr static size_t argument_size = nested_traits::argument_size;
    };

    /**
     *  Specialize for a member function type
     */
    template <typename R, typename... Args, typename X>
    struct function_traits<R(X::*)(Args...)>
    {
        using member_type   = X;
        using return_type   = R;
        using argument_type = std::tuple<Args...>;
        constexpr static size_t argument_size = std::tuple_size_v<argument_type>;
    };

    /**
     *  Specialize for a member function type
     */
    template <typename R, typename... Args, typename X>
    struct function_traits<R(X::*)(Args...) noexcept>
    {
        using member_type   = X;
        using return_type   = R;
        using argument_type = std::tuple<Args...>;
        constexpr static size_t argument_size = std::tuple_size_v<argument_type>;
    };
}

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
        using on_update_system = std::function<void(float)>;
        using on_start_system = std::function<void()>;
        using on_event_system = std::function<void(sf::Event&)>;
        template<typename ...systems_t>
        using container_type = std::tuple<std::vector<systems_t>...>;
        using system_container = container_type<on_update_system, on_start_system, on_event_system>;


        manager() = default;

        template<auto callback>
        void system_wrapper() {
        }
        //template<typename ...systems_t>
        //manager(systems_t&& ...systems) {
        //    emplace(std::forward<systems_t>(systems)...);
        //}

        template<typename system_t, typename ...arguments_t>
        void emplace(arguments_t&& ...args) {
            using container = std::vector<typename system_traits<system_t>::system>;
            std::get<container>(_systems).emplace_back(
                std::bind(
                    &system_t::operator(),
                    std::make_shared<system_t>(std::forward<arguments_t>(args)...),
                    std::placeholders::_1
                )
            );
        }

        //template<typename system_t, typename ...arguments_t>
        //void emplace(arguments_t&& ...args) {
        //    _systems[typeid(typename util::function_traits<system_t>::argument_type)].emplace_back(
        //        std::bind(
        //            &system_t::operator(),
        //            std::make_shared<system_t>(std::forward<arguments_t>(args)...)
        //        )
        //    );
        //}

        template<typename ...args_t>
        void run(args_t&& ...arguments) {
            // for each system system of this condition
            for (auto&& system : std::get<std::vector<std::function<void(args_t...)>>>(_systems)) {
                // run with entities and elapsed time
                //system(std::forward<args_t>(arguments)...);
                system(std::forward<args_t>(arguments)...);
            }
        }
    private:
        system_container _systems;
        //std::unordered_map<run_condition, std::vector<system_type>> _systems;
        using handler_type = void(manager::*)();
        //std::unordered_map<std::type_index, std::vector<std::function<void()>>> _systems;
    };
}
