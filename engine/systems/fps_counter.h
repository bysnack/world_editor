#pragma once

#include <functional>
#include "../entities/entities.h"
#include "../components/render.h"
#include "../components/components.h"
#include "./run_condition.h"
#include "../resources/resources.h"

namespace engine::systems {

    void fps_counter(entities::manager& entities, float elapsed_time, const engine::resources::manager&) {
        auto& text_component = entities.at("fps_counter").get_component<components::text>();
        text_component->setString(std::to_string(static_cast<uint16_t>(1.f / elapsed_time)) + "fps");
    }
}
