#include <SunnyLog/SunnyLog.h>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "opengl.hpp"
#include "ray-manager.hpp"
#include "sfml.hpp"
#include "shader.hpp"

const unsigned int WIN_WIDTH = 1920;
const unsigned int WIN_HEIGHT = 1080;
const unsigned int DEFAULT_RAY_AMOUNT = 500;

void printHelp() {
    std::cout << R"(Usage: ./bin -r <number>
       ./bin --rayamount <number>

Set the ray amount for the operation.

Options:
  -r <number>          Short form for ray amount.
  --rayamount <number> Long form for ray amount.
  -h, --help           Show this help message and exit.

Examples:
  ./bin -r 1000
  ./bin --rayamount 1000

Note: Use either -r OR --rayamount, not both.)"
              << '\n';
}

int main(int argc, char* argv[]) {
    unsigned int rayAmount = DEFAULT_RAY_AMOUNT;

    for (int i = 1; i < argc; i++) {
        std::string_view arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp();
            return 0;
        }
    }

    // We require exactly 3 arguments: program, flag, and value
    if (argc >= 3) {
        std::string_view flag = argv[1];

        if (flag != "-r" && flag != "--rayamount") {
            std::cerr << "Error: unknown flag '" << flag << "'.\n";
            printHelp();
            return EXIT_FAILURE;
        }

        try {
            int temp = std::stoi(argv[2]);
            if (temp < 1) {
                std::cerr << "Error: ray amount must be positive.\n";
                printHelp();
                return EXIT_FAILURE;
            }
            rayAmount = static_cast<unsigned int>(temp);
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: '" << argv[2] << "' is not a number.\n";
            printHelp();
            return EXIT_FAILURE;
        } catch (const std::out_of_range&) {
            std::cerr << "Error: number is too large.\n";
            printHelp();
            return EXIT_FAILURE;
        }
    }

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

        sf.onMouseClick(lightRadius, rayManager);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
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
