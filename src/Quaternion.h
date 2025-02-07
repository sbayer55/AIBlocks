#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

#include "Point3.h"

namespace geometry {

struct Quaternion {
    float w, x, y, z;

    // Constructors
    Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
    Quaternion(const float w, const float x, const float y, const float z) : w(w), x(x), y(y), z(z) {}

    // Create quaternion from axis-angle representation
    static Quaternion from_axis_angle(const float angle_degrees, float x, float y, float z) {
        const float angle = angle_degrees * static_cast<float>(M_PI) / 180.0f;
        const float half_angle = angle * 0.5f;
        const float sin_half = std::sin(half_angle);

        if (const float norm = std::sqrt(x*x + y*y + z*z); norm > 0.0f) {
            x /= norm;
            y /= norm;
            z /= norm;
        }

        return {
            std::cos(half_angle),
            x * sin_half,
            y * sin_half,
            z * sin_half
        };
    }

    // Basic operations
    Quaternion operator*(const Quaternion& q) const {
        return {
            w*q.w - x*q.x - y*q.y - z*q.z,
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w
        };
    }

    Quaternion& operator*=(const Quaternion& q) {
        *this = *this * q;
        return *this;
    }

    [[nodiscard]]
    Quaternion conjugate() const {
        return {w, -x, -y, -z};
    }

    [[nodiscard]]
    float magnitude() const {
        return std::sqrt(w*w + x*x + y*y + z*z);
    }

    // Normalize
    void normalize() {
        if (const float mag = magnitude(); mag > 0.0f) {
            const float inv_mag = 1.0f / mag;
            w *= inv_mag;
            x *= inv_mag;
            y *= inv_mag;
            z *= inv_mag;
        }
    }

    // Get normalized copy
    [[nodiscard]]
    Quaternion normalized() const {
        Quaternion q = *this;
        q.normalize();
        return q;
    }

    // Convert to rotation matrix (3x3)
    void to_rotation_matrix(float matrix[9]) const {
        const float xx = x * x;
        const float xy = x * y;
        const float xz = x * z;
        const float xw = x * w;
        const float yy = y * y;
        const float yz = y * z;
        const float yw = y * w;
        const float zz = z * z;
        const float zw = z * w;

        matrix[0] = 1.0f - 2.0f * (yy + zz);
        matrix[1] = 2.0f * (xy - zw);
        matrix[2] = 2.0f * (xz + yw);

        matrix[3] = 2.0f * (xy + zw);
        matrix[4] = 1.0f - 2.0f * (xx + zz);
        matrix[5] = 2.0f * (yz - xw);

        matrix[6] = 2.0f * (xz - yw);
        matrix[7] = 2.0f * (yz + xw);
        matrix[8] = 1.0f - 2.0f * (xx + yy);
    }

    [[nodiscard]]
    Point3 rotate_point(const Point3& p) const {
        const Quaternion p_quat(0.0f, p.x, p.y, p.z);
        const Quaternion rotated = *this * p_quat * conjugate();
        return Point3{rotated.x, rotated.y, rotated.z};
    }

    [[nodiscard]]
    static Quaternion lerp(const Quaternion& q1, const Quaternion& q2, const float t) {
        const float t1 = 1.0f - t;
        return Quaternion(
            t1 * q1.w + t * q2.w,
            t1 * q1.x + t * q2.x,
            t1 * q1.y + t * q2.y,
            t1 * q1.z + t * q2.z
        ).normalized();
    }

    [[nodiscard]]
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, const float t) {
        Quaternion q2_copy = q2;
        float dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;

        if (dot < 0.0f) {
            q2_copy = Quaternion(-q2.w, -q2.x, -q2.y, -q2.z);
            dot = -dot;
        }

        if (dot > 0.9995f) {
            return lerp(q1, q2_copy, t);
        }

        const float theta = std::acos(dot);
        const float sin_theta = std::sin(theta);
        const float t1 = std::sin((1.0f - t) * theta) / sin_theta;
        const float t2 = std::sin(t * theta) / sin_theta;

        return {
            t1 * q1.w + t2 * q2_copy.w,
            t1 * q1.x + t2 * q2_copy.x,
            t1 * q1.y + t2 * q2_copy.y,
            t1 * q1.z + t2 * q2_copy.z
        };
    }
};

} // geometry

#endif //QUATERNION_H