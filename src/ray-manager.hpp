#pragma once

#include <SFML/System/Vector2.hpp>
#include <glm/glm.hpp>
#include <memory>

#include "opengl.hpp"
#include "ray.hpp"
#include "shader.hpp"

class RayManager {
public:
    RayManager(unsigned int rayAmount, sf::Vector2f rayOrigin, std::vector<Circle>& circles, Opengl& gl)
        : m_gl(gl)
        , m_circlesUniform(circles)
        , m_rayOrigin(rayOrigin)
        , rayShader(initiateRayShader(rayAmount)) {}

    ~RayManager() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
    }

    void updateRayVertices(const sf::Vector2f newOrigin, const float aspectRatio);

    void updateRayOrigin(sf::Vector2f newRayOrigin);
    void checkForIntersectionAndUpdate(float aspectRatio);
    std::optional<sf::Vector2f> findIntersection(const Circle& circle, Ray& ray, float aspectRatio);

    inline const std::vector<float>& getRayVertices() { return m_rayVertices; }

    inline const unsigned int& getVAO() const { return m_VAO; }

    inline const unsigned int& getVBO() const { return m_VBO; }

    inline const sf::Vector2f& getRayOrigin() const { return m_rayOrigin; }

private:
    Opengl& m_gl;
    std::vector<Circle>& m_circlesUniform;
    sf::Vector2f m_rayOrigin; // aka circles[0]
    std::vector<Ray> m_rays;
    std::vector<float> m_rayVertices;
    unsigned int m_VAO, m_VBO;

    void initiateRays(size_t rayAmount);
    std::unique_ptr<Shader> initiateRayShader(unsigned int rayAmount);

    inline float dot(const sf::Vector2f& v1, const sf::Vector2f v2) const { return v1.x * v2.x + v1.y * v2.y; }

public:
    std::unique_ptr<Shader> rayShader;
};
