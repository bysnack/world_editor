#pragma once
#include "entity.h"
#include <uuid/uuid.h>

namespace engine::entities {
    
    class manager {
    public:
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

            return it->second;
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
    private:
        std::unordered_map<std::string, entity> _entities;
    };

}
