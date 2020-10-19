#pragma once

#include "SFML/Graphics.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "./atlas.h"

namespace engine::resources {

    class graphics {
    public:
        const static constexpr uint16_t max_atlas_size = 8000; 

        graphics(std::string_view index_path, std::string_view grhs_path) :
            _atlas{ index_path, grhs_path }
        {
            // retrieve the file
            std::ifstream index{ index_path.data() };

            // start reading line per line
            std::string line;
            while (std::getline(index, line)) {
                // remove the Grh prefix
                line.erase(0, 3);

                // retrieve and parse the grh number
                auto pos = line.find("=");
                uint32_t grh = std::stoi(line.substr(0, pos));
                line.erase(0, pos + 1);
    
                // retrieve and parse the amount of grhs that compose this item
                pos = line.find("-");
                uint32_t size = std::stoi(line.substr(0, pos));
                line.erase(0, pos + 1);

                // the rest of the line is the grh data
                std::istringstream iss{ line };
                switch (size) {
                case 1: {
                    std::array<uint32_t, 4> dimensions;
                    uint32_t graphic_number = 0;
                    for (uint8_t i = 0; i <= dimensions.size(); ++i) {
                        if (!std::getline(iss, line, '-')) {
                            throw std::invalid_argument{ "Invalid grh index value" };
                        }
                        if (i == 0) {
                            graphic_number = std::stoi(line);
                            continue;
                        }
                        dimensions[i - 1] = std::stoi(line);
                    }
                    const sf::IntRect& grh_pos = _atlas[graphic_number];
                    _grhs.emplace(
                        std::piecewise_construct_t{},
                        std::forward_as_tuple(grh),
                        std::forward_as_tuple(grh_pos.left + dimensions[0], grh_pos.top + dimensions[1], dimensions[2], dimensions[3])
                    );
                    break;

                }
                default:
                    break;
                }
            }            
        }

        operator sf::Texture&() {
            return _atlas;
        }

        sf::IntRect operator[](uint32_t idx) {
            return _grhs[idx];
        }
    private:
        atlas _atlas;
        std::unordered_map<uint32_t, sf::IntRect> _grhs;
    };
}
