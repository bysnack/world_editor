#pragma once
#include "component.h"

namespace engine::components {

    struct mesh : public component {
        mesh() = default;
        mesh(sf::IntRect texture, sf::Vector2f offset, sf::Vector2f size, uint32_t priority) :
            component{},
            texture{ texture },
            offset{ offset },
            size{ size },
            priority{ priority }
        {}
    
        sf::IntRect texture;
        sf::Vector2f offset;
        sf::Vector2f size;
        uint32_t priority;
    };
}
