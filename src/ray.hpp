#pragma once
#include <glm/glm.hpp>

#include "types.hpp"

struct Ray {
    glm::vec2 direction;
    glm::vec2 hitPoint;
    float distance;

    Ray(float angle)
        : direction(glm::cos(angle), glm::sin(angle)) {}

    static inline std::vector<Ray> createRays(size_t rayAmount) {
        float anglePerRay = 2.f * M_PI / rayAmount;
        std::vector<Ray> rays;
        rays.reserve(rayAmount);

        for (size_t i = 0; i < rayAmount; i++) {
            rays.emplace_back(i * anglePerRay);
        }
        return rays;
    }

    void castRays(const Circle& sourceObject, const std::vector<Ray>& rays, const std::vector<Circle>& obstacles);
};
