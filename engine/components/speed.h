#pragma once
#include "./component.h"

namespace engine::components {

    struct speed : public component {
        speed(float dt) :
            dt{ dt }
        {}

        float dt;
    };
}