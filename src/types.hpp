#pragma once
#include <glm/glm.hpp>

struct Circle {
public:
    glm::vec2 position;
    float radius;

    Circle(glm::vec2 position, float r)
        : position(position)
        , radius(r) {}

    Circle(float x, float y, float r)
        : position(x, y)
        , radius(r) {}
};
