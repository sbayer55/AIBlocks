#include "RenderComponent.h"
#include <GLFW/glfw3.h>

void RenderComponent::render() const {
    const auto& transform = game_object->get_transform();

    glTranslatef(transform.position.x, transform.position.y, transform.position.z);
    glScalef(transform.scale.x, transform.scale.y, transform.scale.z);

    glBegin(GL_QUADS);
    for (const auto& vertex : vertices_) {
        glColor4f(vertex.r, vertex.g, vertex.b, vertex.a);
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
}
