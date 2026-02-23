#include "ray-manager.hpp"

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <optional>
#include <string>

void RayManager::initiateRays(size_t rayAmount) {
    float anglePerRay = 2.f * std::numbers::pi / rayAmount;
    m_rays.reserve(rayAmount);

    for (size_t i = 0; i < rayAmount; i++) {
        m_rays.emplace_back(i * anglePerRay);
    }
}

void RayManager::updateRayVertices(const sf::Vector2f newOrigin, const float aspectRatio) {
    // NOTE:
    // use aspect-corrected space for calculations
    // but pass in normal UV space to vertices since
    // the vertex shader expects uv space
    sf::Vector2f uvStart = newOrigin;
    // use aspect-corrected space for calculations
    uvStart.x *= aspectRatio;
    m_rayVertices.clear();
    for (const auto& ray : m_rays) {

        m_rayVertices.emplace_back(newOrigin.x);
        m_rayVertices.emplace_back(newOrigin.y);

        // /
        sf::Vector2f end = uvStart + ray.direction * ray.distance;
        // get out of uv space
        end.x /= aspectRatio;
        m_rayVertices.emplace_back(end.x);
        m_rayVertices.emplace_back(end.y);
    }

    updateRayOrigin(newOrigin);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_rayVertices.size() * sizeof(float), m_rayVertices.data());
}

void RayManager::updateRayOrigin(sf::Vector2f newRayOrigin) {
    m_rayOrigin = newRayOrigin;
    if (!m_circlesUniform.empty()) {
        m_circlesUniform[0].position = m_rayOrigin;
        m_gl.shader.useProgram();
        m_gl.shader.setCircles(m_circlesUniform);
    }
}

std::unique_ptr<Shader> RayManager::initiateRayShader(unsigned int rayAmount) {
    initiateRays(rayAmount);

    m_rayVertices.resize(rayAmount * 4, 0.0f);

    m_gl.setupVAOVBO(m_rayVertices, m_VAO, m_VBO);
    const std::string lineVertShaderPath = "../src/vertLine.glsl";
    const std::string lineFragShaderPath = "../src/fragLine.glsl";
    std::unique_ptr<Shader> rayShader = std::make_unique<Shader>(lineVertShaderPath, lineFragShaderPath);

    rayShader->useProgram();
    rayShader->setVec3f("lineColor", 1.0f, 1.0f, 0.0f);
    return rayShader;
}

std::optional<sf::Vector2f> RayManager::findIntersection(const Circle& circle, Ray& ray, float aspectRatio) {
    // Ray: O + t * D   (t >= 0)
    // Circle: |P - C|^2 = r^2
    // NOTE: the function assumes uv space coordinates

    auto correctedOrigin = sf::Vector2f{m_rayOrigin.x * aspectRatio, m_rayOrigin.y};

    // oc: vector from center of circle to rayOrigin
    sf::Vector2f oc = correctedOrigin - circle.position;

    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(oc, ray.direction);
    float c = dot(oc, oc) - circle.radius * circle.radius;

    float disc = b * b - 4.0f * a * c;
    if (disc < 0.0f) {
        ray.distance = 2;
        return std::nullopt;
    }

    float sqrtDisc = std::sqrt(disc);
    float t1 = (-b - sqrtDisc) / (2.0f * a);
    float t2 = (-b + sqrtDisc) / (2.0f * a);

    // Find the smallest positive t
    float t = -1.0f;
    if (t1 > 0.0f) t = t1;
    if (t2 > 0.0f && (t < 0.0f || t2 < t)) t = t2;

    if (t < 0.0f) return std::nullopt;

    ray.distance = t;
    sf::Vector2f res = m_rayOrigin + t * ray.direction;
    res.x /= aspectRatio;
    return res;
}

void RayManager::checkForIntersectionAndUpdate(float aspectRatio) {
    Circle correctedCircle = m_circlesUniform[1];
    correctedCircle.position.x *= aspectRatio;
    for (auto& ray : m_rays) {
        findIntersection(correctedCircle, ray, aspectRatio);
        updateRayVertices(m_rayOrigin, aspectRatio);
    }
}
