#version 330 core
out vec4 FragColor;

struct circle {
        vec2 position;
        float radius;
};

uniform vec2 iResolution;
uniform circle circles[4];
uniform int circleNum;

void main() {
        // Circle rendering:

        vec3 color;
        // Normalized pixel coordinates (from 0 to 1)
        vec2 uv = (gl_FragCoord.xy / iResolution);
        float aspect = iResolution.x / iResolution.y;
        uv.x *= aspect;

        for (int i = 0; i < circleNum; i++) {
                vec2 circlePos = circles[i].position;
                circlePos.x *= aspect; // Apply same correction!

                float distance = distance(uv, circlePos);
                if (distance < circles[i].radius) {
                        color = vec3(1.0, 0.5, 0.2);
                        // break because we are making the pixel colored even if its in both circles
                        break;
                } else {}
        }

        FragColor = vec4(color, 1.0);
}
