#include "sfml.hpp"

#include <SFML/System/Vector2.hpp>

#include "ray-manager.hpp"

void Sfml::onMouseClick(sf::Vector2f origin, float lightRadius, RayManager& rayManager) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float uvX = static_cast<float>(mousePos.x) / window.getSize().x;
    float uvY = 1.0f - static_cast<float>(mousePos.y) / window.getSize().y;

    sf::Vector2f mouseUV = {uvX * aspectRatio, uvY};
    mouseUV -= sf::Vector2f(origin.x * aspectRatio, origin.y);

    if (mouseUV.lengthSquared() <= lightRadius * lightRadius) {
        rayManager.updateRayVertices(mouseUV, aspectRatio);
    }
}
