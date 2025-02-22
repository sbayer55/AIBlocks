#ifndef MOUSE_KEYBOARD_MOVEMENT_COMPONENT_H
#define MOUSE_KEYBOARD_MOVEMENT_COMPONENT_H

#include "GameObject.h"
#include "GLFW/glfw3.h"

class KeyEventHandler {
protected:
    ~KeyEventHandler() = default;

public:
    virtual void on_key_event(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
};

class MouseEventHandler {
protected:
    ~MouseEventHandler() = default;

public:
    virtual void on_mouse_move(GLFWwindow* window, double xpos, double ypos) = 0;
};

class MouseKeyboardMovementComponent : public Component, public KeyEventHandler, public MouseEventHandler  {
public:
    void on_key_event(GLFWwindow *window, int key, int scancode, int action, int mods) override;
    void on_mouse_move(GLFWwindow* window, double xpos, double ypos) override;
private:
    float speed = 10.0f;
    double mouse_sensitivity = 0.1f;
    double pitch = 0.0f;
    double yaw = 0.0f;
};

inline void MouseKeyboardMovementComponent::on_key_event(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    Transform& transform = gameObject->get_transform();
    glm::vec3 position = transform.position;

    const float delta = speed * 0.1f;

    switch (key) {
        case GLFW_KEY_W:
            position.z += delta;
            break;
        case GLFW_KEY_S:
            position.z -= delta;
            break;
        case GLFW_KEY_A:
            position.x += delta;
            break;
        case GLFW_KEY_D:
            position.x -= delta;
            break;
        default:
            break;;
    }

    transform.position = position;

}

inline void MouseKeyboardMovementComponent::on_mouse_move(GLFWwindow *window, const double xpos, const double ypos) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    yaw += (xpos - static_cast<double>(width) / 4.0f) * mouse_sensitivity;
    pitch += (ypos - static_cast<double>(height) / 4.0f) * mouse_sensitivity;


    Transform& transform = gameObject->get_transform();

    // Convert euler angles to quaternion
    const glm::quat quat_x = angleAxis(glm::radians(static_cast<float>(pitch)), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::quat quat_y = angleAxis(glm::radians(static_cast<float>(yaw)), glm::vec3(0.0f, 1.0f, 0.0f));

    transform.rotation = quat_y * quat_x;
}

#endif //MOUSE_KEYBOARD_MOVEMENT_COMPONENT_H
