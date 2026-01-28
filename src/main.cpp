#include <SunnyLog/SunnyLog.h>
#include <glad/glad.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <vector>

#include "shader.hpp"

#define WIN_WIDTH 720
#define WIN_HEIGHT 480
#define DT 1.0f / 60.0f

struct Circle {
    int x;
    int y;
    int r;

    Circle(float x, float y, float r)
        : x(x)
        , y(y)
        , r(r) {}
};

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

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode({WIN_WIDTH, WIN_HEIGHT}), "raytracing", sf::State::Windowed, settings);

    window.setVerticalSyncEnabled(true);
    window.setSize(sf::Vector2u(WIN_WIDTH, WIN_HEIGHT));
    window.setPosition(sf::Vector2i(WIN_WIDTH * 3 / 4, WIN_HEIGHT / 8 - 20));
    window.setMinimumSize(sf::Vector2u(400, 300));
    bool isWindowActive = window.setActive(true);

    window.setFramerateLimit(120); // Call this once after creating your window

    if (!gladLoadGL()) {
        SunnyLog::log("Error during gladLoadGl");
    }

    const std::string vertexShaderPath = "../src/shader.vert";
    const std::string fragmentShaderPath = "../src/shader.frag";
    Shader shader{vertexShaderPath, fragmentShaderPath};

    // 2. Create and bind VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (window.isOpen()) {
        glClearColor(0.0f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.useProgram();

        // Set uniform AFTER useProgram()
        int resLocation = glGetUniformLocation(shader.getProgram(), "iResolution");
        glUniform2f(resLocation, WIN_WIDTH, WIN_HEIGHT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shader.deleteProgram();

    return 0;
}
