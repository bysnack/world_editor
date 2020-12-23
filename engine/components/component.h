#pragma once

namespace engine::components {
    struct component {
        component() = default;
        uint32_t id() {
            static uint32_t id = 0x01;
            return id << 1;
        }
        virtual ~component() = default;
    };
}
