#ifndef POINT3_H
#define POINT3_H

namespace geometry {

struct Point3 {
    const float x, y, z;

    Point3 operator+(const Point3& offset) const {
        return Point3(x + offset.x, y + offset.y, z + offset.z);
    }

    Point3 operator*(const Point3& offset) const {
        return Point3(x * offset.x, y * offset.y, z * offset.z);
    }

    Point3 operator*(const float scale) const {
        return Point3(x * scale, y * scale, z * scale);
    }

    friend Point3 operator*(const float scale, const Point3& point) {
        return point * scale;
    }

};

static constexpr Point3 P_ZERO = {0.0f, 0.0f, 0.0f};

} // geometry

#endif //POINT3_H
