#include "Cube.h"

#include <vector>
#include <array>
#include <GLFW/glfw3.h>

#include "Vertex.h"
#include "Transform.h"

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
void Cube::draw() const {
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

void Cube::glRotateF() const {
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

void Cube::rotate(const glm::quat& rotation) {
    const auto newRotation = this->transform_.rotation * rotation;
    this->transform_.rotation = newRotation;
}

void Cube::rotateX(const float deg) {
    this->rotate(xAxisRotation(deg));
}

void Cube::rotateY(const float deg) {
    this->rotate(yAxisRotation(deg));
}

void Cube::rotateZ(const float deg) {
    this->rotate(zAxisRotation(deg));
}
