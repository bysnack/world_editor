#pragma once
#include "./component.h"

namespace engine::components {

    struct position : public component {
        operator sf::Vector2f() {
            return coords;
        }

        sf::Vector2f coords;
    };
}