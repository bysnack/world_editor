#pragma once
#include "component.h"

namespace engine::components {

    struct render : public component {
        sf::IntRect texture;
        sf::Vector2f offset;
        sf::Vector2f size;
        uint32_t priority;
    };
}
