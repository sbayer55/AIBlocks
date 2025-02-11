#include <GLFW/glfw3.h>
#include <iostream>

#include "src/Cube.h"
#include "src/Point3.h"

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char* argv[]) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    // Set error callback
    glfwSetErrorCallback(error_callback);

    // Create window
    constexpr int width = 640;
    constexpr int height = 480;
    GLFWwindow* window = glfwCreateWindow(width, height, "Chunk Preview", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwSetKeyCallback(window, key_callback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Enable vsync
    glfwSwapInterval(1);

    // Set up OpenGL viewport
    glViewport(0, 0, width, height);

    // Set up 2D rendering
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -100, 100);
    glMatrixMode(GL_MODELVIEW);

    float rotation = 0.0f;

    const auto q = geometry::Quaternion(0, 0, 0, 1);
    const float scale = 40.0f;
    const std::array cubes = {
        geometry::Cube({50, 50, 0}, q, scale),
        geometry::Cube({50, 200, 0}, q, scale),
        geometry::Cube({200, 50, 0}, q, scale),
    };
    // const auto cube2 = geometry::Cube(scale) + geometry::Point3{100, 300, 0};

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& cube : cubes) {
            glLoadIdentity();
            // cube.rotate_ = {rotation, 0, 0, 0};
            cube.draw();
        }

        rotation += 0.5f;
        if (rotation >= 360.0f) {
            rotation = 0.0f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
