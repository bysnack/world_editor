#pragma once

#include "./atlas.h"
#include "./fonts.h"
#include "../config.h"


namespace engine::resources {

    class manager {
    public:
        manager(const config& config) :
            _resources{
                atlas{ std::filesystem::path(config.init), std::filesystem::path(config.graphics) },
                fonts{ config.fonts }
            }
        {}

        template<class resource_t>
        const resource_t& get() const {
            return std::get<resource_t>(_resources);
        }

    private:
        std::tuple<atlas, fonts> _resources;
    };
}
