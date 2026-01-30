
#include <glm/glm.hpp>

struct Circle {
public:
    glm::vec2 position;
    float radius;

    Circle(float x, float y, float r)
        : position(x, y)
        , radius(r) {}
};
