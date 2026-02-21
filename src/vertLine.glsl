#version 330 core
layout(location = 0) in vec2 aPos; // now in uv space [0,1]

void main() {
        // Convert [0,1] to [-1,1] (NDC)
        vec2 ndc = aPos * 2.0 - 1.0;
        gl_Position = vec4(ndc, 0.0, 1.0);
}
