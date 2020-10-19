#pragma once
#include "./component.h"

namespace engine::components {

    struct size : public component {
        operator sf::Vector2f() {
            return vector;
        }

        sf::Vector2f vector;
    };
}
