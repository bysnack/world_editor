#pragma once

#include <functional>
#include "../entities/entities.h"
#include "../components/render.h"
#include "../components/components.h"
#include "./run_condition.h"

namespace engine::systems {

    struct render {
        sf::RenderWindow& target;

        void operator()(engine::entities::manager& entities, float elapsed_time) {
            std::map<uint32_t, std::vector<std::tuple<sf::VertexArray*, components::position*>>> drawables;

            // for each entity
            for (auto&& [id, entity] : entities.view<components::mesh, components::position>()) {
                // if we have the render component
                auto [mesh, position] = entity.get_components<components::mesh, components::position>();
                if (mesh && position) {
                    drawables[mesh->priority].push_back({ &mesh->drawable, position });
                }
            }
            // select all elements in this priority
            for (auto&& [priority, components] : drawables) {

                // for every one of those elements
                for (auto&& [drawable, position] : components) {
                    // draw it in the correct view
                    //target.setView(component.get()->camera);
                    sf::FloatRect bounds = drawable->getBounds();
                    auto transform = sf::Transform{}
                        .translate(position->coords.x - bounds.left, position->coords.y - bounds.top);
                        //.scale();
                    
                    sf::RenderStates states{ std::move(transform) };

                    target.draw(*drawable, std::move(states));
                }
            }
        }
    };
}
