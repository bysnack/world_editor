#pragma once

// The function of this class is to create a texture atlas using the graphics in the 
// graphics folder, for this we want:
// 1 - Number of grhs
// 2 - Position X1 Y1 of the grh relatively to the atlas
// 3 - Size of the grh
// 4 - Type of the grh (Head - Body - Weapon - Grh - Fx)
// The atlas max dimensions are 8000*8000, the idea is to create a new atlas when needed
// that way the client only need a few textures to work 
#include "SFML/Graphics.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include "../utils/binary.h"

namespace engine::resources {

    class atlas {
    public:
        using grh_ind_t = std::tuple<uint32_t, uint16_t, uint16_t, uint16_t, uint16_t>;
        atlas(std::filesystem::path index_path, std::filesystem::path grhs_path) :
            render_texture{ std::make_unique<sf::RenderTexture>() }
        {
            const auto max_text_size = sf::Texture::getMaximumSize();

            // define the multiset we'll be using to order the textures by height
            auto comp = [](auto&& lhs, auto&& rhs){
                return std::get<4>(lhs.second) > std::get<4>(rhs.second);
            };
            std::multiset<std::pair<int32_t, grh_ind_t>, decltype(comp)> container{ comp };

            // retrieve the file
            std::ifstream index{ index_path / "Graficos.ind", std::ios::binary };

            while (auto data = utils::read_bytes_from_stream<uint32_t, uint32_t, uint16_t, uint16_t, uint16_t, uint16_t>(index)) {
                auto [grh, name, left, top, width, height] = *data;

                // insert into the set
                container.emplace(grh, std::forward_as_tuple(name, left, top, width, height));
            }

            // initialie the render texture where we will render all our graphics
            render_texture->create(max_text_size, max_text_size);
            render_texture->clear(sf::Color::Transparent);

            // define the calculated positions
            sf::Vector2u position{ 0, 0 };
            float height_gap = std::get<4>(container.begin()->second);


            // create the image we are going to load and the texture we are going to fill with it
            sf::Image image;
            sf::Texture text;

            // for each grh
            uint32_t current = 0;
            for (const auto& [number, data] : container) {
                // retrieve texture size and calculate save height gap
                auto [name, left, top, width, height] = data;

                if (current != name) {
                    // load image from source file
                    if (!image.loadFromFile(grhs_path / (std::to_string(name) + ".png"))) {
                        continue;
                    }

                    // mask from black to remove background
                    image.createMaskFromColor(sf::Color::Black);
                    current = name;
                    
                    // load the texture
                    if (!text.loadFromImage(image)) {
                        continue;
                    }
                }

                // we can still create a new row?
                if (position.y + height >= max_text_size - 1) {
                    throw std::runtime_error{ "Render texture is too big." };
                }

                // are we out of space to shape right?
                if (position.x + width >= max_text_size - 1) {
                    // jump to the next row and start again
                    position.y += height_gap;
                    position.x = 0.f;
                    height_gap = height;
                }
                
                // load it into a sprite
                sf::Sprite sprite{ text, sf::IntRect{ left, top, width, height } };

                // position the sprite and render it into the render structure
                sprite.setPosition({ static_cast<float>(position.x), static_cast<float>(position.y) });
                render_texture->draw(sprite);
                render_texture->display();

                // store the position for this grh
                _positions.emplace(
                    std::piecewise_construct_t{},
                    std::forward_as_tuple(number),
                    std::forward_as_tuple(position.x, position.y, width, height)
                );

                // shift right
                position.x += width;
            }
        }

        operator const sf::Texture&() const {
            return render_texture->getTexture();
        }

        operator const sf::Texture*() const {
            return &render_texture->getTexture();
        }

        auto begin()  const { return _positions.begin();    }
        auto end()    const { return _positions.end();      }
        auto cbegin() const { return _positions.cbegin();   }
        auto cend()   const { return _positions.cend();     }

        sf::IntRect operator[](uint32_t idx) const {
            return _positions.at(idx);
        }

    private:
        std::map<uint32_t, sf::IntRect> _positions;
        std::unique_ptr<sf::RenderTexture> render_texture;
    };
}
