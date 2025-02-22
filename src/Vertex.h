#ifndef VERTEX_H
#define VERTEX_H
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct Vertex {
    const float x, y, z;
    const float r, g, b, a;

    explicit Vertex(const glm::vec3& v, const glm::vec4& c = {1.0f, 1.0f, 1.0f, 1.0f}) :
    x(v.x), y(v.y), z(v.z),
    r(c.r), g(c.g), b(c.b), a(c.a) {}
};

static const Vertex P_ZERO = Vertex({0, 0, 0}, {1, 1, 1, 1});

#endif //VERTEX_H
