#include "engine/core.hpp"

struct player_move{
    player_move(engine::entities::manager& e) : entities{e} {}
    engine::entities::manager& entities;

    void operator()(float elapsed_time) {
        auto& player = entities["player"];
        auto p_pos = player.get_component<engine::components::position>();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            p_pos->coords += { 1000.f * elapsed_time, 0.f };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            p_pos->coords += { -1000.f * elapsed_time, 0.f };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            p_pos->coords += { 0.f, -1000.f * elapsed_time };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            p_pos->coords += { 0.f, 1000.f * elapsed_time };
        }
    }
};


int main() {
    engine::config config{ "Source/INIT/", "Source/Graphics/" , "Source/Fonts/", 1920, 1080 };
    engine::core core{ std::move(config) };
    core.systems.emplace<player_move>(core.entities);

    const auto& atlas = core.resources.get<engine::resources::atlas>();

    // lets make a tile map
    for (uint32_t j = 0; j < 100; ++j) {
        for (uint32_t i = 0; i < 100; ++i) {
            core.entities.create("floor" + std::to_string(i + j * 100), 
                engine::components::mesh{atlas[12439 + (i % 4) + ((j % 4) * 4)], { 32.f, 32.f }, { 0.f, 0.f }, 2 },
                engine::components::position{{ i * 32.f, j * 32.f }}
            );
        }
    }

    core.entities.create("player",
        engine::components::mesh{atlas[12469], { 32.f, 32.f }, { 32.f, 32.f }, 4 },
        engine::components::position{{ 32.f, 32.f }},
        engine::components::size{{ 32.f, 32.f }},
        engine::components::speed{1000.f}
    );

    core.run();

    return 0;
}