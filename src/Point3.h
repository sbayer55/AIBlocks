//
// Created by Steven Bayer on 2/6/25.
//

#ifndef POINT3_H
#define POINT3_H

namespace geometry {

struct Point3 {
    float x, y, z;

    Point3 operator+(const Point3& offset) const {
        return {x + offset.x, y + offset.y, z + offset.z};
    }

    Point3 operator*(const Point3& offset) const {
        return {x * offset.x, y * offset.y, z * offset.z};
    }

    Point3 operator*(const float scale) const {
        return {x * scale, y * scale, z * scale};
    }

    friend Point3 operator*(const float scale, const Point3& point) {
        return point * scale;
    }

};

} // geometry

#endif //POINT_H
