#include <SunnyLog/SunnyLog.h>

#include <cstdint>
#include <glm/vec2.hpp>
#include <vector>

#include "opengl.hpp"
#include "sfml.hpp"

#define WIN_WIDTH 720
#define WIN_HEIGHT 480
#define DT 1.0f / 60.0f

int main() {
    std::vector<float> vertices = {
        // positions        // colors
        -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, // top-left
        1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, 0.0f  // bottom-right
    };

    std::vector<uint> indices = {
        0, 1, 2, // first triangle
        1, 2, 3  // second triangle
    };

    std::vector<Circle> circles;
    circles.emplace_back(0.5, 0.5, 0.1);
    circles.emplace_back(1.0, 1.0, 0.05);

    Sfml sf{WIN_WIDTH, WIN_HEIGHT};

    const std::string vertexShaderPath = "../src/shader.vert";
    const std::string fragmentShaderPath = "../src/shader.frag";
    Opengl gl{vertexShaderPath, fragmentShaderPath};
    gl.setupOpengl(vertices, indices);
    gl.shader.useProgram();
    gl.shader.setVec2f("iResolution", WIN_WIDTH, WIN_HEIGHT);
    gl.shader.setCircles(circles);
    gl.shader.setInt("circleNum", circles.size());

    while (sf.window.isOpen()) {
        glClearColor(0.0f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gl.shader.useProgram();
        gl.draw();

        sf.window.display();
    }

    gl.deleteEverything();
    return 0;
}
