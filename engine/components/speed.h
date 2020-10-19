#pragma once
#include "./component.h"

namespace engine::components {

    struct speed : public component {
        float dt;
    };
}