#pragma once

#include <functional>
#include <gsl/span>
#include "../entities/entities.h"
#include "../components/render.h"
#include "../components/components.h"
#include "./run_condition.h"

namespace engine::systems {

    struct render : public entities::observer {
        render(sf::RenderWindow& window, entities::manager& manager, const resources::manager& res) :
            observer{ manager },
            target{ window },
            resources{ res },
            entities{ manager }
        {
        }

        void on_entity_created(const std::string& id, entities::entity& entity) final {
            auto [mesh, position] = entity.get_components<components::mesh, components::position>();
            if (mesh && position) {
                auto& varr = drawables[mesh->priority];
                auto size = varr.getVertexCount();
                if (size == 0) {
                    varr.setPrimitiveType(sf::Quads);
                }

                auto transform = sf::Transform{}
                    .translate(mesh->offset)
                    .translate(*position)
                    .scale(mesh->texture.width, mesh->texture.height);

                varr.resize(size + 4);

                auto* quad = &varr[size];

                quad[0].position = transform.transformPoint({ 0.f, 0.f });
                quad[1].position = transform.transformPoint({ 1.f, 0.f });
                quad[2].position = transform.transformPoint({ 1.f, 1.f });
                quad[3].position = transform.transformPoint({ 0.f, 1.f });

                quad[0].texCoords = { static_cast<float>(mesh->texture.left),                      static_cast<float>(mesh->texture.top) };                
                quad[1].texCoords = { static_cast<float>(mesh->texture.left + mesh->texture.width),   static_cast<float>(mesh->texture.top) };
                quad[2].texCoords = { static_cast<float>(mesh->texture.left + mesh->texture.width),   static_cast<float>(mesh->texture.top + mesh->texture.height) };
                quad[3].texCoords = { static_cast<float>(mesh->texture.left),                      static_cast<float>(mesh->texture.top + mesh->texture.height) };

                vertex_view.emplace(
                    std::piecewise_construct_t{},
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(mesh->priority, size)
                );
            }
        }

        void on_entity_removed(const std::string& id) final {
            
        }

        sf::RenderWindow& target;
        std::map<uint32_t, sf::VertexArray> drawables;
        std::unordered_map<std::string, std::tuple<uint32_t, uint32_t>> vertex_view;
        const resources::manager& resources;
        entities::manager& entities;

        void operator()(float elapsed_time) {
            for (const auto& elem : vertex_view) {
                entities.at(elem.first).with_components([this, &elem](components::mesh* mesh, components::position* position){
                    if (!mesh || !position) {
                        return;
                    }

                    const auto& [id, data] = elem;
                    auto [prio, index] = data;
                    auto& varr = drawables.at(prio);
                    auto* span = &varr[index];
                    // check that the positions actually changed beore doing any changes
                    if (span[0].position.x != position->coords.x + mesh->offset.x || span[0].position.y != position->coords.y + mesh->offset.y) {
                        auto transform = sf::Transform{}
                            .translate(mesh->offset)
                            .translate(*position)
                            .scale(mesh->texture.width, mesh->texture.height);

                        span[0].position = transform.transformPoint({ 0.f, 0.f });
                        span[1].position = transform.transformPoint({ 1.f, 0.f });
                        span[2].position = transform.transformPoint({ 1.f, 1.f });
                        span[3].position = transform.transformPoint({ 0.f, 1.f });
                    }
                });
            }

            // select all elements in this priority
            for (auto&& [priority, varr] : drawables) {
                sf::RenderStates states{ resources.get<engine::resources::atlas>() };
                target.draw(varr, std::move(states));
            }
        }
    };
}
