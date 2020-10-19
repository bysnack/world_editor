#pragma once

#include "SFML/System/Vector2.hpp"
#include <type_traits>

template<typename value_t, typename = std::enable_if_t<std::is_arithmetic_v<value_t>>>
struct vector : public sf::Vector2<value_t> {
    using sf::Vector2<value_t>::Vector2;
    vector& scale(value_t value) {
        this->x *= value;
        this->y *= value;

        return *this;
    }

    vector& scale(const sf::Vector2<value_t>& value) {
        this->x *= value;
        this->y *=value;

        return *this;
    }

    vector& traslate(const sf::Vector2<value_t>& coordinates) {
        this->x += coordinates.x;
        this->y += coordinates.y;

        return *this;
    }

    operator sf::Vector2<value_t>() {
        return { this->x, this->y };
    }
};
