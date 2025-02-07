#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <ranges>

#include "Point2.h"
#include "Point3.h"
#include "ProjectionHelper.h"

namespace geometry {

struct Cube {
    const std::vector<Point3> vertices_;
    const std::vector<int> indices_;
    const SDL_FColor color_;

    static constexpr SDL_FColor kDefaultColor = {255, 0, 0, 180};

    static constexpr std::array<Point3, 8> kUnitCube = {{
        {-.5f, -.5f, -.5f},
        {0.5f, -.5f, -.5f},
        {-.5f, 0.5f, -.5f},
        {0.5f, 0.5f, -.5f},
        {-.5f, -.5f, 0.5f},
        {0.5f, -.5f, 0.5f},
        {-.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
    }};

    static constexpr std::array<int, 36> kUnitCubeIndices = {{
        // Front face
        0, 1, 2,
        2, 1, 3,
        // Back face
        4, 6, 5,
        5, 6, 7,
        // Left face
        0, 2, 4,
        4, 2, 6,
        // Right face
        1, 5, 3,
        3, 5, 7,
        // Top face
        2, 3, 6,
        6, 3, 7,
        // Bottom face
        0, 4, 1,
        1, 4, 5
    }};

    Cube operator+(const Point3& offset) const {
        return Cube(vertices_
            | std::views::transform([=](const Point3& p) { return p + offset; })
            | std::ranges::to<std::vector>()
        );
    }

    friend Cube operator+(const Point3& offset, const Cube& cube) {
        return cube + offset;
    }

    Cube operator*(const Point3& scale) const {
        return Cube(vertices_
            | std::views::transform([=](const Point3& p) { return p * scale; })
            | std::ranges::to<std::vector>()
        );
    }

    friend Cube operator*(const Point3& scale, const Cube& cube) {
        return cube * scale;
    }

    Cube operator*(const float scale) const {
        return Cube(vertices_
            | std::views::transform([=](const Point3& p) { return p * scale; })
            | std::ranges::to<std::vector>()
        );
    }

    friend Cube operator*(const float scale, const Cube& cube) {
        return cube * scale;
    }


    explicit Cube(const float scale = 1.0f):
        vertices_(kUnitCube
            | std::views::transform([=](const Point3& p) { return p * scale; })
            | std::ranges::to<std::vector>()
        ),
        color_(kDefaultColor),
        indices_(kUnitCubeIndices | std::ranges::to<std::vector>()) {
    }

    explicit Cube(const std::vector<Point3> &vertices, const float scale = 1.0f):
        vertices_(vertices),
        color_(kDefaultColor),
        indices_(kUnitCubeIndices | std::ranges::to<std::vector>()) {
    }

    void draw(SDL_Renderer* renderer) const {
        const std::vector<Point2> isoPoints = to_isometric(this->vertices_, {0.0f, 0.0f}, 1.0f);

        std::vector<SDL_Vertex> vertices;
        vertices.reserve(isoPoints.size());

        for (size_t i = 0; i < isoPoints.size(); ++i) {
            float g = (1. / isoPoints.size()) * i;
            std::cout << g << std::endl;
            vertices.push_back(SDL_Vertex{
                .position = {isoPoints[i].x, isoPoints[i].y},
                .color = { 1., g, g, .8 },
                .tex_coord = {0, 0}
            });
        }

        SDL_RenderGeometry(
            renderer,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            this->indices_.data(),
            static_cast<int>(this->indices_.size())
        );
    }
};

} // chunk

#endif //CUBE_H
