#pragma once
#include "entity.h"
#include <uuid/uuid.h>

namespace engine::entities {

    struct observer {
        template<typename provider_t>
        observer(provider_t& provider, std::enable_if_t<
            std::is_same_v<
                decltype(std::declval<provider_t>()._observers),
                std::vector<observer*>
            >
        >* = 0)
        {
            provider._observers.push_back(this);
        }

        observer() = delete;

        virtual void on_entity_created(const std::string& id, entity& target) = 0;
        virtual void on_entity_removed(const std::string& id) = 0;
    };
    
    class manager {
    public:
        friend class observer;

        template<class ...components_t>
        auto &create(std::string id, components_t&& ...components) {
            //uuid_t newUUID;
            //uuid_generate(newUUID);
            auto [it, res] = _entities.emplace(
                std::piecewise_construct_t{},
                //std::forward_as_tuple(newUUID, newUUID + sizeof newUUID / sizeof newUUID[0] ),
                std::forward_as_tuple(std::move(id)),
                std::forward_as_tuple(std::forward<components_t>(components)...)
            );

            // on_entity_created all observers
            for (auto* ob : _observers) {
                ob->on_entity_created(it->first, it->second);
            }

            return it->second;
        }

        void remove(const std::string& id) {
            _entities.erase(id);
            for (auto* ob : _observers) {
                ob->on_entity_removed(id);
            }
        }

        auto get_if(std::function<bool(entity&)> condition) {
            return std::find_if(_entities.begin(), _entities.end(), [&condition](auto&& elem){
                return condition(elem.second);
            });
        }

        auto begin() noexcept { return _entities.begin(); }
        auto end() noexcept { return _entities.end(); }
        auto cbeign() const noexcept { return _entities.cbegin(); }
        auto cend() const noexcept { return _entities.cend(); }

        entity& operator[](const std::string& id) {
            return _entities[id];
        }

        entity& at(const std::string& id) {
            return _entities.at(id);
        }

        template<class ...components_t>
        std::vector<std::tuple<std::string_view, entity&>> view() {
            std::vector<std::tuple<std::string_view, entity&>> view;
            for (auto&& [key, value] : _entities) {
                if (value.has_components<components_t...>()) {
                    view.push_back({ key, value });
                }
            }

            return view;
        }

        template<typename handler_t, class ...components_t>
        void with_components(handler_t handler) {
            for (auto&& [key, value] : _entities) {
                handler(value);
            }
        }
    private:
        std::map<std::string, entity> _entities;
        std::vector<observer*> _observers;
    };

}
