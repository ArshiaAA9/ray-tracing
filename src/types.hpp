#pragma once
#include <SFML/System/Vector2.hpp>
#include <glm/glm.hpp>

struct Circle {
public:
    sf::Vector2f position;
    float radius;

    Circle(sf::Vector2f position, float r)
        : position(position)
        , radius(r) {}

    Circle(float x, float y, float r)
        : position(x, y)
        , radius(r) {}
};
