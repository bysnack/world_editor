#pragma once
#include "component.h"

namespace engine::components {

    struct mesh : public component {
        enum class texture_mode {
            simple,
            tile,
            stretch
        };

        mesh(const std::tuple<const sf::IntRect&, const sf::Texture&>& grh, const sf::Vector2f& s, sf::Vector2f pos, texture_mode mode, uint32_t prio = 0) :
            offset{ std::move(pos) },
            size{ std::move(s) },
            drawable{ sf::Quads },
            priority{ prio }
        {
            // retrieve the texture dimensions
            const auto& dimensions = std::get<0>(grh);

            // calculate the number of rows and columns we can fit in this size
            uint32_t rows = size.y / dimensions.height;
            uint32_t columns = size.x / dimensions.width;

            drawable.resize(rows * columns * 4);

            for (uint32_t i = 0; i < rows; ++i) {
                for (uint32_t j = 0; j < columns; ++j) {
                    // define a transform for scaling our quad position and texture
                    auto transform = sf::Transform{}
                        .translate(offset)
                        .translate(dimensions.width * j, dimensions.height * i)
                        .scale(dimensions.width, dimensions.height);

                    // retrieve a pointer to a quad
                    sf::Vertex* quad = &drawable[(j + i * columns) * 4];

                    // define its 4 corners
                    quad[0].position = transform.transformPoint({ 0.f, 0.f });
                    quad[1].position = transform.transformPoint({ 1.f, 0.f });
                    quad[2].position = transform.transformPoint({ 1.f, 1.f });
                    quad[3].position = transform.transformPoint({ 0.f, 1.f });

                    // define its 4 texture coordinates
                    switch (mode) {
                    case texture_mode::simple:
                        if (i + j == 0) {
                            quad[0].texCoords = { static_cast<float>(dimensions.left),                      static_cast<float>(dimensions.top) };                
                            quad[1].texCoords = { static_cast<float>(dimensions.left + dimensions.width),   static_cast<float>(dimensions.top) };
                            quad[2].texCoords = { static_cast<float>(dimensions.left + dimensions.width),   static_cast<float>(dimensions.top + dimensions.height) };
                            quad[3].texCoords = { static_cast<float>(dimensions.left),                      static_cast<float>(dimensions.top + dimensions.height) };
                        }
                        break;
                    case texture_mode::tile:
                        quad[0].texCoords = { static_cast<float>(dimensions.left),                      static_cast<float>(dimensions.top) };                
                        quad[1].texCoords = { static_cast<float>(dimensions.left + dimensions.width),   static_cast<float>(dimensions.top) };
                        quad[2].texCoords = { static_cast<float>(dimensions.left + dimensions.width),   static_cast<float>(dimensions.top + dimensions.height) };
                        quad[3].texCoords = { static_cast<float>(dimensions.left),                      static_cast<float>(dimensions.top + dimensions.height) };
                        break;
                    case texture_mode::stretch:
                        quad[0].texCoords = { static_cast<float>(dimensions.left),                      static_cast<float>(dimensions.top) };                
                        quad[1].texCoords = { static_cast<float>(dimensions.left + dimensions.width),   static_cast<float>(dimensions.top) };
                        quad[2].texCoords = { static_cast<float>(dimensions.left + dimensions.width),   static_cast<float>(dimensions.top + dimensions.height) };
                        quad[3].texCoords = { static_cast<float>(dimensions.left),                      static_cast<float>(dimensions.top + dimensions.height) };
                        break;
                    }
                }
            }
        }

        mesh(sf::Color color, sf::Vector2f s, sf::Vector2f pos, uint32_t prio = 0) :
            offset{ std::move(pos) },
            size{ std::move(s) },
            drawable{ sf::Quads, 4 },
            priority{ prio }
        {

            auto transform = sf::Transform{}
                .translate(offset)
                .scale(size);

            // define its 4 corners
            drawable[0].position = transform.transformPoint({ 0.f, 0.f });
            drawable[1].position = transform.transformPoint({ 1.f, 0.f });
            drawable[2].position = transform.transformPoint({ 1.f, 1.f });
            drawable[3].position = transform.transformPoint({ 0.f, 1.f });

            // define color
            drawable[0].color = color;
            drawable[1].color = color;
            drawable[2].color = color;
            drawable[3].color = color;
        }

        sf::Vector2f offset;
        sf::Vector2f size;
        sf::VertexArray drawable;
        uint32_t priority;
    };
}
