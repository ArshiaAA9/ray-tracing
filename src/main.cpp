#include <SunnyLog/SunnyLog.h>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include "opengl.hpp"
#include "ray-manager.hpp"
#include "sfml.hpp"
#include "shader.hpp"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

int main() {
    std::vector<float> vertices = {
        // positions
        -1.0f, 1.0f,  // top-left
        1.0f,  1.0f,  // top-right
        -1.0f, -1.0f, // bottom-left
        1.0f,  -1.0f, // bottom-right
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, // first triangle
        1, 2, 3  // second triangle
    };
    Sfml sf{WIN_WIDTH, WIN_HEIGHT};

    sf::Vector2f position1 = {0.2, 0.5};
    float lightRadius = 0.04;
    sf::Vector2f position2 = {0.7, 0.3};
    sf::Vector2f position3 = {0.2, 0.6};
    sf::Vector2f position4 = {0.3, 0.1};
    std::vector<Circle> circles;

    circles.emplace_back(position1, lightRadius);
    circles.emplace_back(position2, 0.08);
    circles.emplace_back(position3, 0.08);
    circles.emplace_back(position4, 0.08);

    unsigned int rayAmount = 1000;

    const std::string vertexShaderPath = "../src/vertex.glsl";
    const std::string fragmentShaderPath = "../src/frag.glsl";
    Opengl gl{vertexShaderPath, fragmentShaderPath};
    gl.setupOpengl(vertices, indices);
    gl.shader.useProgram();
    gl.shader.setVec2f("iResolution", WIN_WIDTH, WIN_HEIGHT);
    gl.shader.setCircles(circles);
    gl.shader.setInt("circleNum", circles.size());

    RayManager rayManager{rayAmount, position1, circles, gl};
    rayManager.updateRayVertices(position1, sf.aspectRatio);

    // rayManager.checkForIntersectionAndUpdate(sf.aspectRatio);

    while (sf.window.isOpen()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            sf.window.close();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf.onMouseClick(lightRadius, rayManager);
        }
        rayManager.checkForIntersectionAndUpdate(sf.aspectRatio);

        glClearColor(0.f, 0.0f, 0.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl.shader.useProgram();
        gl.draw();
        rayManager.rayShader->useProgram();
        gl.drawLine(rayManager.getVAO(), rayAmount * 2);

        sf.window.display();
    }
    // class destructors free the resources themselves

    return 0;
}
