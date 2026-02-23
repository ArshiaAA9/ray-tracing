#include "sfml.hpp"

#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "ray-manager.hpp"

void Sfml::onMouseClick(float lightRadius, RayManager& rayManager) {
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    float uvX = mousePos.x / window.getSize().x;
    float uvY = 1.0f - mousePos.y / window.getSize().y;
    sf::Vector2f mouseUV = {uvX, uvY};
    sf::Vector2f mouseCorrected = {uvX * aspectRatio, uvY};

    const sf::Vector2f& rayOrigin = rayManager.getRayOrigin();
    sf::Vector2f rayOriginCorrected = {rayOrigin.x * aspectRatio, rayOrigin.y};

    // calculations must be done in aspect-corrected space
    sf::Vector2f diff = mouseCorrected - rayOriginCorrected;

    // this check is faling
    if (diff.lengthSquared() <= lightRadius * lightRadius) {
        // but the vertices expect uv space coord
        rayManager.updateRayVertices(mouseUV, aspectRatio);
    }
}
