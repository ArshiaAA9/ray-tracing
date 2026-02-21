#include <SunnyLog/SunnyLog.h>

#include <SFML/System/Angle.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include "opengl.hpp"
#include "ray.hpp"
#include "sfml.hpp"
#include "shader.hpp"

#define WIN_WIDTH 720
#define WIN_HEIGHT 480
#define DT 1.0f / 60.0f

int main() {
    std::vector<float> vertices = {
        // positions
        -1.0f, 1.0f,  0.0f, // top-left
        1.0f,  1.0f,  0.0f, // top-right
        -1.0f, -1.0f, 0.0f, // bottom-left
        1.0f,  -1.0f, 0.0f, // bottom-right
    };

    std::vector<uint> indices = {
        0, 1, 2, // first triangle
        1, 2, 3  // second triangle
    };

    std::vector<Circle> circles;
    glm::vec2 position1 = {0.1, 0.4};
    glm::vec2 position2 = {0.7, 0.6};

    circles.emplace_back(position1, 0.01);
    circles.emplace_back(position2, 0.15);

    std::vector<float> rayVertices;
    int rayAmount = 18;
    auto rays = Ray::createRays(rayAmount);
    for (const auto& ray : rays) {

        // Start point (center)
        rayVertices.push_back(position1.x);
        rayVertices.push_back(position1.y);

        // End point
        glm::vec2 end = position1 + ray.direction * 1.f;
        rayVertices.push_back(end.x);
        rayVertices.push_back(end.y);
    }

    Sfml sf{WIN_WIDTH, WIN_HEIGHT};

    const std::string vertexShaderPath = "../src/vertex.glsl";
    const std::string fragmentShaderPath = "../src/frag.glsl";
    Opengl gl{vertexShaderPath, fragmentShaderPath};
    gl.setupOpengl(vertices, indices);
    gl.shader.useProgram();
    gl.shader.setVec2f("iResolution", WIN_WIDTH, WIN_HEIGHT);
    gl.shader.setCircles(circles);
    gl.shader.setInt("circleNum", circles.size());

    unsigned rayVAO;
    unsigned rayVBO;
    gl.setupVAOVBO(rayVertices, rayVAO, rayVBO);
    const std::string lineVertShaderPath = "../src/vertLine.glsl";
    const std::string lineFragShaderPath = "../src/fragLine.glsl";
    Shader lineShader = {lineVertShaderPath, lineFragShaderPath};
    lineShader.useProgram();
    lineShader.setVec3f("lineColor", 1.0f, 1.0f, 0.0f);

    while (sf.window.isOpen()) {
        glClearColor(1.0f, 1.0f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gl.shader.useProgram();
        gl.draw();
        lineShader.useProgram();
        gl.drawLine(rayVAO, rayAmount * 2);

        sf.window.display();
    }

    gl.deleteEverything();
    glDeleteVertexArrays(1, &rayVAO);
    glDeleteBuffers(1, &rayVBO);

    return 0;
}
