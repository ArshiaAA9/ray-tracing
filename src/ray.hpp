#pragma once
#include <SFML/System/Vector2.hpp>
#include <glm/glm.hpp>

struct Ray {
    float slope;
    sf::Vector2f direction;
    sf::Vector2f hitPoint;
    float distance;

    Ray(float angle)
        : slope(glm::tan(angle))
        , direction(glm::cos(angle), glm::sin(angle))
        , distance(2.f) {}
};
