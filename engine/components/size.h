#pragma once
#include "./component.h"

namespace engine::components {

    struct size : public component {
        size(sf::Vector2f vector) :
            vector{ std::move(vector) }
        {}

        operator sf::Vector2f() {
            return vector;
        }

        sf::Vector2f vector;
    };
}
