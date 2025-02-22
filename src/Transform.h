#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "glm/fwd.hpp"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"

struct Transform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    static Transform of( const glm::vec3& position) {
        return {position, {0.f, 1.f, 0.f, 0.f}, glm::vec3(1.0f)};
    }

    static Transform of( const glm::vec3& position, const float scale) {
        return {position, {0.f, 1.f, 0.f, 0.f}, glm::vec3(scale)};
    }
};

static glm::quat xAxisRotation(const float deg) {
    return angleAxis(glm::radians(deg), glm::vec3(1.0f, 0.0f, 0.0f));
}

static glm::quat yAxisRotation(const float deg) {
    return angleAxis(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

static glm::quat zAxisRotation(const float deg) {
    return angleAxis(glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));
}

static constexpr Transform TRANSFORM_ZERO = {
    {0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0}
};

#endif //TRANSFORM_H
