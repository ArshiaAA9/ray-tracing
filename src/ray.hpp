#pragma once
#include <SFML/System/Vector2.hpp>
#include <glm/glm.hpp>

struct Ray {
    float slope;
    sf::Vector2f direction;

    // for future if we ever decided to implement light bounces
    sf::Vector2f hitPoint;

    Ray(float angle)
        : slope(glm::tan(angle))
        , direction(glm::cos(angle), glm::sin(angle))
        , m_distance(2.f) {}

    inline void setDistance(float distance) {
        if (distance < m_distance) m_distance = distance;
    }

    inline void resetDistance() { m_distance = 2.0f; }

    inline float getDistance() const { return m_distance; }

private:
    float m_distance;
};
