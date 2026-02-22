#include <SunnyLog/SunnyLog.h>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include "glm/ext/vector_float2.hpp"
#include "opengl.hpp"
#include "ray-manager.hpp"
#include "sfml.hpp"
#include "shader.hpp"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
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
    Sfml sf{WIN_WIDTH, WIN_HEIGHT};

    std::vector<Circle> circles;
    sf::Vector2f position1 = {0.2, 0.5};
    float lightRadius = 0.04;
    sf::Vector2f position2 = {0.7, 0.6};

    circles.emplace_back(position1, lightRadius);
    circles.emplace_back(position2, 0.15);

    uint rayAmount = 100;
    RayManager rayManager{rayAmount};
    rayManager.updateRayVertices(position1, sf.aspectRatio);

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
    gl.setupVAOVBO(rayManager.getRayVertices(), rayVAO, rayVBO);
    const std::string lineVertShaderPath = "../src/vertLine.glsl";
    const std::string lineFragShaderPath = "../src/fragLine.glsl";
    Shader lineShader = {lineVertShaderPath, lineFragShaderPath};
    lineShader.useProgram();
    lineShader.setVec3f("lineColor", 1.0f, 1.0f, 0.0f);

    while (sf.window.isOpen()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            sf.window.close();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf.onMouseClick(sf::Vector2f{position1.x, position1.y}, lightRadius, rayManager);
            // TODO: after updating vertex i need to move them to the shader again
            // same for the circle
        }

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
