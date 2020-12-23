#pragma once

namespace engine::components {

    struct texture : public component {
        operator const sf::Texture&() {
            return data;
        }

        sf::Texture data;
    };
}
