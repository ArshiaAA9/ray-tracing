#include "ray-manager.hpp"

#include <SFML/System/Vector2.hpp>

void RayManager::initiateRays(size_t rayAmount) {
    float anglePerRay = 2.f * M_PI / rayAmount;
    m_rays.reserve(rayAmount);

    for (size_t i = 0; i < rayAmount; i++) {
        m_rays.emplace_back(i * anglePerRay);
    }
}

void RayManager::updateRayVertices(const sf::Vector2f newOrigin, const float aspectRatio) {
    // Start point (center)
    sf::Vector2f uvStart = newOrigin;
    uvStart.x *= aspectRatio;
    for (const auto& ray : m_rays) {

        m_rayVertices.push_back(newOrigin.x);
        m_rayVertices.push_back(newOrigin.y);

        // End point
        // add the direction
        sf::Vector2f end = uvStart + ray.direction * ray.distance;
        // get out of uv space
        end.x /= aspectRatio;
        m_rayVertices.push_back(end.x);
        m_rayVertices.push_back(end.y);
    }
}
