#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <array>
#include <ranges>

#include "GameObject.h"
#include "Vertex.h"
#include "Transform.h"

struct Color {
    float r, g, b, a;
};

class Cube: public GameObject {
public:
    explicit Cube(
        const Transform& transform = TRANSFORM_ZERO,
        const Color& color = kDefaultColor)
        : vertices_(kUnitCube | std::ranges::to<std::vector>())
        , transform_(transform)
        , color_(color) {
    }

    void draw() const;

    void setColor(const Color& color) { color_ = color; }

    void rotate(const glm::quat&);
    void rotateX(float deg);
    void rotateY(float deg);
    void rotateZ(float deg);

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

#endif //CUBE_H
