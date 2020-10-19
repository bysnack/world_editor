#pragma once
#include "component.h"

namespace engine::components {

    struct render {
        render(const std::tuple<const sf::IntRect&, const sf::Texture&>& grh, const sf::Vector2f& size, const sf::Vector2f& position, uint32_t prio = 0) :
            states{ &std::get<1>(grh) },
            priority{ prio }
        {
            const auto& dimensions = std::get<0>(grh);
            sf::VertexArray quad{ sf::Quads, 4 };

            // define a transform for scaling our quad position and texture
            sf::Transform transform;
            transform.scale(size);
            transform.translate(position);

            // define its 4 corners
            quad[0].position = transform.transformPoint({ 0.f, 0.f });
            quad[1].position = transform.transformPoint({ 1.f, 0.f });
            quad[2].position = transform.transformPoint({ 1.f, 1.f });
            quad[3].position = transform.transformPoint({ 0.f, 1.f });

            // define its 4 texture coordinates
            quad[0].texCoords = { static_cast<float>(dimensions.left),                      static_cast<float>(dimensions.top) };                
            quad[1].texCoords = { static_cast<float>(dimensions.left + dimensions.width),   static_cast<float>(dimensions.top) };
            quad[2].texCoords = { static_cast<float>(dimensions.left + dimensions.width),   static_cast<float>(dimensions.top + dimensions.height) };
            quad[3].texCoords = { static_cast<float>(dimensions.left),                      static_cast<float>(dimensions.top + dimensions.height) };

            drawable = std::make_unique<sf::VertexArray>(std::move(quad));
        }

        sf::RenderStates states;
        sf::Vector2f position;
        std::shared_ptr<sf::Drawable> drawable;
        uint32_t priority;
    };
}
