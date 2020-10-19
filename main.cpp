#include "engine/core.hpp"

void player_move(engine::entities::manager& entities, float elapsed_time) {

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

void player_collisions(engine::entities::manager& entities, float) {
    auto [p_mesh, p_size, p_position] = entities["player"].get_components<engine::components::mesh, engine::components::size, engine::components::position>();

    for (auto&& [id, entity] : entities) {
        if (id == "player") {
            continue;
        }

        if (auto collision = entity.get_component<engine::components::collision>()) {
            if (collision->box.intersects({ *p_position, *p_size })) {
                std::cout << "Game Over\n";
            }
        }
    }
}

int main() {
    engine::config config{ "Source/INIT/", "Source/Graphics/" , "Source/Fonts/", 1920, 1080 };
    engine::core core{ std::move(config) };

    core.systems.emplace(&player_move);
    core.systems.emplace(&player_collisions);

    auto create_wall = [&core](std::string name, float x, float y, float width, float height){
        core.entities.create(std::move(name),
            engine::components::mesh{ sf::Color::Green, { width, height }, { x, y }, 1 },
            engine::components::collision{ .box = { x, y, width, height } },
            engine::components::position{ .coords= { x, y } }
        );
    };

    create_wall("north", 0.f, 0.f, 1920.f, 32.f);
    create_wall("south", 0.f, 1080.f - 32.f, 1920.f, 32.f);
    create_wall("west", 0.f, 0.f, 32.f, 1080.f);
    create_wall("east", 1920.f - 32.f, 0.f, 32.f, 1080.f);
    core.entities.create("player",
        engine::components::mesh{ sf::Color::Red, { 32.f, 32.f }, { 32.f, 32.f }, 2 },
        engine::components::position{ .coords = { 32.f, 32.f }},
        engine::components::size{ .vector = { 32.f, 32.f }},
        engine::components::speed{ .dt = 1000.f }
    );

    core.run();

    return 0;
}