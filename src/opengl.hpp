#pragma once
#include <glad/glad.h>

#include <vector>

#include "shader.hpp"

class Opengl {
public:
    Opengl(const std::string& vertexShaderPath, const std::string& fragShaderPath)
        : shader(vertexShaderPath, fragShaderPath) {}

    void setupOpengl(std::vector<float>& vertices, std::vector<uint>& indices);
    void setupVAOVBO(std::vector<float>& vertices, unsigned int& VAO, unsigned int& VBO);

    inline void draw() {
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    inline void drawLine(unsigned int VAO, unsigned int lineAmount) {
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, lineAmount);
        glBindVertexArray(0);
    }

    inline void deleteEverything() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        shader.deleteProgram();
    }

    Shader shader;

private:
    unsigned int m_VAO, m_VBO, m_EBO;
};
