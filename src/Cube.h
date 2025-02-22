#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <array>
#include <ranges>
#include <GLFW/glfw3.h>

#include "Vertex.h"
#include "Transform.h"

struct Color {
    float r, g, b, a;
};

class Cube {
public:
    explicit Cube(
        const Transform& transform = TRANSFORM_ZERO,
        const Color& color = kDefaultColor)
        : vertices_(kUnitCube | std::ranges::to<std::vector>())
        , transform_(transform)
        , color_(color) {
    }

    // Public methods
    void draw() const;

    // Getters and setters
    void setTransform(const Transform& transform) { transform_ = transform; }
    void setColor(const Color& color) { color_ = color; }

    void rotate(const glm::quat&);
    void rotateX(float deg);
    void rotateY(float deg);
    void rotateZ(float deg);

    [[nodiscard]]
    const Transform& getTransform() const { return transform_; }
    [[nodiscard]]
    const Color& getColor() const { return color_; }

private:
    static constexpr Color kDefaultColor = {1.0f, 0.0f, 0.0f, 0.7f};

    static const std::array<Vertex, 24> kUnitCube;

    void glRotateF() const;

    std::vector<Vertex> vertices_;
    Transform transform_;
    Color color_;
};

const std::array<Vertex, 24> Cube::kUnitCube = {{
    // Front face
    Vertex({-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 0.6f}),
    Vertex({0.5f, -0.5f, 0.5f}, {0.8f, 1.0f, 1.0f, 0.6f}),
    Vertex({0.5f, 0.5f, 0.5f}, {0.6f, 1.0f, 1.0f, 0.6f}),
    Vertex({-0.5f, 0.5f, 0.5f}, {0.4f, 1.0f, 1.0f, 0.6f}),

    // Back face
    Vertex({-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 0.6f}),
    Vertex({-0.5f, 0.5f, -0.5f}, {1.0f, 0.8f, 1.0f, 0.6f}),
    Vertex({0.5f, 0.5f, -0.5f}, {1.0f, 0.6f, 1.0f, 0.6f}),
    Vertex({0.5f, -0.5f, -0.5f}, {1.0f, 0.4f, 1.0f, 0.6f}),

    // Top face
    Vertex({-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 0.6f}),
    Vertex({-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.8f, 0.6f}),
    Vertex({0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.6f, 0.6f}),
    Vertex({0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 0.4f, 0.6f}),

    // Bottom face
    Vertex({-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 0.6f}),
    Vertex({0.5f, -0.5f, -0.5f}, {0.8f, 0.8f, 1.0f, 0.6f}),
    Vertex({0.5f, -0.5f, 0.5f}, {0.6f, 0.6f, 1.0f, 0.6f}),
    Vertex({-0.5f, -0.5f, 0.5f}, {0.4f, 0.4f, 1.0f, 0.6f}),

    // Right face
    Vertex({0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 0.6f}),
    Vertex({0.5f, 0.5f, -0.5f}, {1.0f, 0.8f, 0.8f, 0.6f}),
    Vertex({0.5f, 0.5f, 0.5f}, {1.0f, 0.6f, 0.6f, 0.6f}),
    Vertex({0.5f, -0.5f, 0.5f}, {1.0f, 0.4f, 0.4f, 0.6f}),

    // Left face
    Vertex({-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 0.6f}),
    Vertex({-0.5f, -0.5f, 0.5f}, {0.8f, 1.0f, 0.8f, 0.6f}),
    Vertex({-0.5f, 0.5f, 0.5f}, {0.6f, 1.0f, 0.6f, 0.6f}),
    Vertex({-0.5f, 0.5f, -0.5f}, {0.4f, 1.0f, 0.4f, 0.6f}),
}};

// Implementation of draw method
inline void Cube::draw() const {
    glTranslatef(transform_.position.x, transform_.position.y, transform_.position.z);
    // this->glRotateF();
    glScalef(transform_.scale.x, transform_.scale.y, transform_.scale.z);

    glBegin(GL_QUADS);
    for (const auto& vertex : vertices_) {
        glColor4f(vertex.r, vertex.g, vertex.b, vertex.a);
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
}

inline void Cube::glRotateF() const {
    // Convert quaternion to axis-angle representation
    const auto rotation = this->transform_.rotation;
    const float angle = glm::degrees(2.0f * std::acos(rotation.w));

    // Avoid division by zero when extracting axis
    glm::vec3 axis;
    if (const float s = 1.0f - rotation.w * rotation.w; s > 0.0001f) {
        axis = glm::vec3(rotation.x, rotation.y, rotation.z) / std::sqrt(s);
    } else {
        // Default axis if rotation is near identity
        axis = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glRotatef(angle, axis.x, axis.y, axis.z);
}

inline void Cube::rotate(const glm::quat& rotation) {
    const auto newRotation = this->transform_.rotation * rotation;
    this->transform_.rotation = newRotation;
}

inline void Cube::rotateX(const float deg) {
    this->rotate(xAxisRotation(deg));
}

inline void Cube::rotateY(const float deg) {
    this->rotate(yAxisRotation(deg));
}

inline void Cube::rotateZ(const float deg) {
    this->rotate(zAxisRotation(deg));
}

#endif //CUBE_H