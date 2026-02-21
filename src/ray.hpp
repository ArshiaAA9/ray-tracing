#pragma once
#include <SFML/System/Vector2.hpp>
#include <glm/glm.hpp>

struct Ray {
    sf::Vector2f direction;
    sf::Vector2f hitPoint;
    float distance;

    Ray(float angle)
        : direction(glm::cos(angle), glm::sin(angle))
        , distance(2.f) {}
};
