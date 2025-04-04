
#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H
#include "Color.h"
#include "GameObject.h"


class MeshComponent final : public Component {
public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    Color color;

    MeshComponent(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices) :
        vertices(vertices), indices(indices), color({1.0f, 0.0f, 0.0f, 0.7f}) {}

    MeshComponent() = default;
};

#endif //MESHCOMPONENT_H
