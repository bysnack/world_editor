#pragma once
#include "./component.h"

namespace engine::components {

    struct position : public component {
        position() = default;
        position(sf::Vector2f coords) :
            coords{ std::move(coords) }
        {}

        operator sf::Vector2f() {
            return coords;
        }

        sf::Vector2f coords;
    };
}