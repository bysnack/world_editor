#pragma once

#include <unordered_map>
#include <cstdint>

namespace engine::managers {

    template<class entity_t, class accessor_t = uint32_t>
    class manager {
    public:
        entity_t& get(accessor_t accessor) {
            return _container.at(accessor);
        }

    protected:
        std::unordered_map<accessor_t, entity_t> _container;
    };
}
