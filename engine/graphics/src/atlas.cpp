#include "graphics/atlas.hpp"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <set>

namespace graphics {

namespace {

struct Static {
  std::uint32_t graph;
  std::uint32_t left;
  std::uint32_t top;
  std::uint32_t width;
  std::uint32_t height;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Static, graph, left, top, width, height)
};

struct Animation {
  std::vector<std::uint32_t> sequence;
  std::uint32_t speed;
};

auto read_graphs_initialization_file(const std::filesystem::path &path) {
  // retrieve the file
  std::ifstream graphics{path};
  auto graphics_json = nlohmann::json::parse(graphics);
  static constexpr auto comp = [](const auto &lhs, const auto &rhs) {
    return lhs.second.height > rhs.second.height;
  };
  std::multiset<std::pair<std::uint32_t, Static>, decltype(comp)> grh_container{
      comp};

  for (const auto &[key, val] : graphics_json.items()) {
    if (val["type"].get<std::string>() == "animation") {
      // Skip animations for now
      continue;
    }

    grh_container.insert({std::stoi(key), val.get<Static>()});
  }

  return grh_container;
}
} // namespace

Atlas::Atlas(const std::filesystem::path &init,
             const std::filesystem::path &dir) {
  const auto grh_container = read_graphs_initialization_file(init);
  const auto max_text_size = sf::Texture::getMaximumSize();

  m_render_texture.create(max_text_size, max_text_size);
  m_render_texture.clear(sf::Color::Transparent);

  // The current position of the graph to draw
  sf::Vector2u position{0, 0};
  // The gap used to jump to the next row, since graphs are
  // ordered by height using the height of the first element
  // ensures that there's enough space
  std::uint32_t height_gap = grh_container.begin()->second.height;
  // The id of the image used for the current processed texture
  std::uint32_t current_texture = 0;
  sf::Image image;
  sf::Texture text;

  for (const auto &[id, static_grh] : grh_container) {

    if (static_grh.graph != current_texture) {
      // Images hold tilesets, so the image is already loaded whenever the
      // current graphic is the same as the one for this grh
      if (!image.loadFromFile(dir /
                              (std::to_string(static_grh.graph) + ".png"))) {
        continue;
      }

      // Remove the black background by masking the image before loading it
      image.createMaskFromColor(sf::Color::Black);
      current_texture = static_grh.graph;

      if (!text.loadFromImage(image)) {
        continue;
      }
    }

    if (position.x + static_grh.width >= max_text_size - 1) {
      // We ran out of space on this row, jump to the next one and start from
      // the first column
      position.y += height_gap;
      position.x = 0.f;
      height_gap = static_grh.height;
    }

    if (position.y + static_grh.height >= max_text_size - 1) {
      // Surpasing the texture size when jumping into a new row will fail
      // TODO: Create a new render texture here when this happens
      throw std::runtime_error{"Render texture is too big"};
    }

    // Draw the loaded grh into the render texture
    sf::Sprite sprite{text, sf::IntRect{static_cast<int>(static_grh.left),
                                        static_cast<int>(static_grh.top),
                                        static_cast<int>(static_grh.width),
                                        static_cast<int>(static_grh.height)}};
    sprite.setPosition(
        {static_cast<float>(position.x), static_cast<float>(position.y)});
    m_render_texture.draw(sprite);
    m_render_texture.display();

    m_grhs.try_emplace(id, static_cast<int>(position.x),
                       static_cast<int>(position.y),
                       static_cast<int>(static_grh.width),
                       static_cast<int>(static_grh.height));

    // Shift right before continuing with the following grh
    position.x += static_grh.width;
  }
}

} // namespace graphics
