#ifndef PROJECTIONHELPER_H
#define PROJECTIONHELPER_H

#include <vector>

#include "Point2.h"
#include "Point3.h"

namespace geometry {

// Constants for isometric projection`x
constexpr float ISO_ANGLE = 35.264f;  // arctan(1/sqrt(2)) in degrees
constexpr float DEG_TO_RAD = M_PI / 180.0f;

[[nodiscard]]
inline std::vector<Point2> to_isometric(const std::vector<Point3>& points, const float scale = 1.0f) {
    std::vector<Point2> projected;
    projected.reserve(points.size());

    // Pre-calculate cosine and sine values
    const float cos_angle = std::cos(ISO_ANGLE * DEG_TO_RAD);
    const float sin_angle = std::sin(ISO_ANGLE * DEG_TO_RAD);

    for (const auto& p : points) {
        // Isometric transformation:
        // First rotate 45° around Y axis, then rotate 35.264° (arctan(1/√2)) around X axis

        // Rotate around Y axis by 45 degrees
        const float x1 = p.x * cos_angle - p.z * sin_angle;
        const float z1 = p.x * sin_angle + p.z * cos_angle;

        // Rotate around X axis by ISO_ANGLE
        const float y2 = p.y * cos_angle - z1 * sin_angle;
        // const float z2 = p.y * sin_angle + z1 * cos_angle;

        // Project to 2D and apply scale
        projected.push_back({
            x1 * scale,
            y2 * scale
        });
    }

    return projected;
}

// Overload that allows specifying a center point for the projection
[[nodiscard]]
inline std::vector<Point2> to_isometric(
    const std::vector<Point3>& points,
    const Point2& center,
    const float scale = 1.0f
) {
    auto projected = to_isometric(points, scale);

    // Translate all points relative to the center
    for (auto&[x, y] : projected) {
        x += center.x;
        y += center.y;
    }

    return projected;
}

// Optional: Helper function to create an isometric grid
[[nodiscard]]
inline std::vector<Point3> create_isometric_grid(
    const int width,
    const int height,
    const float cell_size = 1.0f
) {
    std::vector<Point3> grid_points;
    grid_points.reserve((width + 1) * (height + 1));

    for (int y = 0; y <= height; ++y) {
        for (int x = 0; x <= width; ++x) {
            grid_points.push_back({
                x * cell_size,
                0.0f,
                y * cell_size
            });
        }
    }

    return grid_points;
}

} // geometry

#endif //PROJECTIONHELPER_H
