#pragma once

#include <SFML/Graphics/Font.hpp>
#include <filesystem>

namespace engine::resources {

    struct fonts {
        fonts(std::string_view path) {
            for (auto&& font : std::filesystem::directory_iterator{ path }) {
                const auto& path = font.path();
                if (path.extension() == ".ttf") {
                    _fonts[path].loadFromFile(path);
                }
            }
        }

        const sf::Font& get(const std::string& name) const {
            return _fonts.at(name);
        }
        
    private:
        std::unordered_map<std::string, sf::Font> _fonts;
    };
}
