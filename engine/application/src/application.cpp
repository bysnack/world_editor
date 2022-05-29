#include "application/application.hpp"

#include "core/core.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace sfml {

namespace {

Views make_base_views(const std::unique_ptr<sf::RenderWindow> &window) {
  const auto window_size = sf::Vector2f{window->getSize()};
  auto views = Views{
      Camera{sf::View{sf::FloatRect{0.f, 0.f, window_size.x, window_size.y}}},
      Camera{sf::View{sf::FloatRect{0.f, 0.f, window_size.x, window_size.y}}},
      Camera{sf::View{sf::Vector2f{16.f, 16.f}, sf::Vector2f{32.f, 32.f}}},
  };
  const auto desktop = sf::VideoMode::getDesktopMode();
  views.renderer.view.setViewport({.16f, .16f, .83f, .70f});
  views.selected.view.setViewport({0.01, 0.01f, 0.14, 0.14f});
  return views;
}

std::unique_ptr<sf::RenderWindow> make_window() {
  auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode{1024, 768},
                                                   "Engine 2", sf::Style::None);
  window->setPosition(sf::Vector2i{0, 0});
  window->setFramerateLimit(0);
  window->setVerticalSyncEnabled(true);
  auto desktop = sf::VideoMode::getDesktopMode();
  window->setPosition(
      {static_cast<int>(desktop.width / 2 - window->getSize().x / 2),
       static_cast<int>(desktop.height / 2 - window->getSize().y / 2)});
  return window;
}

tgui::GuiSFML make_gui(const std::unique_ptr<sf::RenderWindow> &window) {
  return {*window};
}

std::chrono::milliseconds make_elapsed_time() { return {}; }

EventHandler make_event_handler() { return {}; }

} // namespace

Application::Application() {
  register_dependency(&make_window);
  register_dependency(&make_base_views);
  register_dependency(&make_gui);
  register_dependency(&make_event_handler);
  register_dependency(&make_elapsed_time);
}

void Application::run() {
  auto &injector = m_engine.initialize();

  auto &target = injector.resolve<const std::unique_ptr<sf::RenderWindow> &>();
  auto &gui = injector.resolve<tgui::GuiSFML &>();
  auto &views = injector.resolve<Views &>();
  auto &event_handler = injector.resolve<EventHandler &>();
  auto &elapsed_time = injector.resolve<std::chrono::milliseconds &>();

  // start the game loop
  while (target->isOpen()) {
    auto start = std::chrono::steady_clock::now();
    // listen for events
    while (event_handler.poll_event(*target)) {
      gui.handleEvent(event_handler.event.back());
    }

    target->setView(views.application.view);
    target->clear(sf::Color::Black);
    gui.draw();
    m_engine.run_systems();
    target->display();
    elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
  }
}

} // namespace sfml