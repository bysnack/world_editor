#pragma once

#include "SFML/Graphics.hpp"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <set>

namespace graphics {

class Atlas {
public:
  Atlas(const std::filesystem::path &init, const std::filesystem::path &dir);
  const std::map<std::uint32_t, sf::IntRect> &grhs() const { return m_grhs; }
  const sf::Texture &texture() const { return m_render_texture.getTexture(); }

private:
  std::map<std::uint32_t, sf::IntRect> m_grhs;
  sf::RenderTexture m_render_texture;
};
} // namespace graphics
