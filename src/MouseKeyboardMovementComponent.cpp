#include "MouseKeyboardMovementComponent.h"

void MouseKeyboardMovementComponent::on_key_event(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    Transform& transform = game_object->get_transform();

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        return;
    }

    glm::vec3 direction;
    switch (key) {
        case GLFW_KEY_W:
            direction = glm::vec3(0.0f, 0.0f, -1.0f);
        break;
        case GLFW_KEY_S:
            direction = glm::vec3(0.0f, 0.0f, 1.0f);
        break;
        case GLFW_KEY_A:
            direction = glm::vec3(-1.0f, 0.0f, 0.0f);
        break;
        case GLFW_KEY_D:
            direction = glm::vec3(1.0f, 0.0f, 0.0f);
        break;
        case GLFW_KEY_Z:
            direction = glm::vec3(0.0f, -1.0f, 0.0f);
        break;
        case GLFW_KEY_SPACE:
            direction = glm::vec3(0.0f, 1.0f, 0.0f);
        break;
        default:
            return;
    }

    // Rotate the direction vector by the object's rotation
    const glm::vec3 rotated_direction = transform.rotation * direction;

    // Update position
    transform.position += rotated_direction;
}

void MouseKeyboardMovementComponent::on_mouse_move(GLFWwindow *window, const double xpos, const double ypos) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    const float x_delta = static_cast<float>(xpos) - static_cast<float>(width) / 4.0f;
    const float y_delta = static_cast<float>(ypos) - static_cast<float>(height) / 4.0f;

    const float yaw_lr_radians = glm::radians(x_delta * mouse_sensitivity);
    const float pitch_ud_radians = glm::radians(y_delta * mouse_sensitivity);

    // Convert euler angles to quaternion
    constexpr auto y_axis = glm::vec3(0.0f, -1.0f, 0.0f);
    constexpr auto x_axis = glm::vec3(-1.0f, 0.0f, 0.0f);

    const glm::quat yaw_lr_rotation = angleAxis(yaw_lr_radians, y_axis);
    const glm::quat pitch_ud_rotation = angleAxis(pitch_ud_radians, x_axis);

    Transform& transform = game_object->get_transform();
    transform.rotation = glm::normalize(transform.rotation * pitch_ud_rotation * yaw_lr_rotation);
}
