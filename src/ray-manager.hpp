#pragma once

#include <glm/glm.hpp>

#include "ray.hpp"

class RayManager {
public:
    RayManager(uint rayAmount) { initiateRays(rayAmount); }

    void updateRayVertices(const sf::Vector2f newOrigin, const float aspectRatio);

    inline std::vector<float>& getRayVertices() { return m_rayVertices; }

private:
    void initiateRays(size_t rayAmount);
    glm::vec2 m_rayOrigin;
    std::vector<Ray> m_rays;
    std::vector<float> m_rayVertices;
};
