
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "GameObject.h"
#include "Vertex.h"
#include "Color.h"

class RenderComponent : public Component {
public:
    explicit RenderComponent(
        const std::vector<Vertex>& vertices,
        const Color& color = kDefaultColor)
        : vertices_(vertices)
        , color_(color) {
    }

    void render() const;
    void setColor(const Color& color) { color_ = color; }
    [[nodiscard]] const Color& getColor() const { return color_; }

private:
    static constexpr Color kDefaultColor = {1.0f, 0.0f, 0.0f, 0.7f};

    std::vector<Vertex> vertices_;
    Color color_;
};




#endif //RENDERCOMPONENT_H
