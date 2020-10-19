#pragma once
#include "./component.h"

namespace engine::components {

    struct collision : public component {
        sf::FloatRect box;
    };
}