#pragma once
#include <tuple>
#include <memory>
#include "./render.h"
#include "./camera.h"
#include "./font.h"
#include "./text.h"
#include "./texture.h"
#include "./rectangle.h"
#include "./mesh.h"
#include "./collision.h"
#include "./position.h"
#include "./size.h"
#include "./speed.h"

namespace engine::components {
    template<class ...components_t>
    using component_list = std::tuple<std::shared_ptr<components_t>...>;
    using components = component_list<render, camera, font, text, texture, rectangle, mesh, collision, position, size>;
}
