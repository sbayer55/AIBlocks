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

class MouseKeyboardMovementComponent final : public Component, public KeyEventHandler, public MouseEventHandler  {
public:
    void on_key_event(GLFWwindow *window, int key, int scancode, int action, int mods) override;
    void on_mouse_move(GLFWwindow* window, double xpos, double ypos) override;
private:
    float speed = 1.0f;
    float mouse_sensitivity = 0.1f;
};

#endif //MOUSE_KEYBOARD_MOVEMENT_COMPONENT_H
