#version 330 core
out vec4 FragColor;

struct circle {
        vec2 position;
        float radius;
};

uniform vec2 iResolution;
uniform circle circles[2];

in vec3 fragColor;

void main() {
        // Normalized pixel coordinates (from 0 to 1)
        vec2 uv = (gl_FragCoord.xy / iResolution) * 2.0 - 1.0;
        float aspect = iResolution.x / iResolution.y;
        uv.x *= aspect;

        float distance = 1.0 - length(uv);
        distance = step(0.0, distance);

        FragColor = vec4(fragColor, distance);
}
