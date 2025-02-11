#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <ranges>

#include "Point2.h"
#include "Point3.h"
#include "Quaternion.h"

namespace geometry {

struct Cube {
    const std::vector<Point3> vertices_;
    Point3 translate_;
    mutable Quaternion rotate_;
    Point3 scale_;
    const struct Color {
        float r, g, b, a;
    } color_;

    static constexpr Color kDefaultColor = {1.0f, 0.0f, 0.0f, 0.7f}; // Red with 0.7 alpha

    static constexpr std::array<Point3, 24> kUnitCube = {{
        // Front face
        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},

        // Back face
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},

        // Top face
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, -0.5f},

        // Bottom face
        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},

        // Right face
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},

        // Left face
        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},

    }};

    explicit Cube(
        const Point3& translate = P_ZERO,
        const Quaternion rotate = {0, 0, 0, 0},
        const float scale = 1.0f,
        const Color& color = kDefaultColor):
        vertices_(kUnitCube | std::ranges::to<std::vector>()),
        translate_(translate),
        rotate_(rotate),
        scale_({scale, scale, scale}),
        color_(color) {
    }

    void draw() const {
        glTranslatef(translate_.x, translate_.y, translate_.z);
        // glRotatef(rotate_.w, rotate_.x, rotate_.y, rotate_.z);
        glScalef(scale_.x, scale_.y, scale_.z);

        glBegin(GL_QUADS);

        for (size_t i = 0; i < vertices_.size(); i++) {
            // const float g = 1.0f / static_cast<float>(vertices_.size()) * static_cast<float>(i);
            // glColor4f(1.0f, g, g, 0.8f);
            glColor4f(color_.r, color_.g, color_.b, color_.a);

            const auto&[x, y, z] = vertices_[i];
            glVertex3f(x, y, z);
        }

        glEnd();

    }
};

} // geometry

#endif //CUBE_H