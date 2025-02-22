#ifndef FOO_H
#define FOO_H
#include <iostream>

#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/io.hpp"

inline void fooErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

inline void foo() {
    glfwInit();
    glfwSetErrorCallback(fooErrorCallback);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Foo", nullptr, nullptr);
    glEnable(GL_DEPTH_TEST);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height); // Make sure to set the viewport!

        glMatrixMode(GL_PROJECTION);
        float aspectRatio = (float)width / (float)height;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
        glLoadMatrixf(value_ptr(projection));

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(0.0f, 0.0f, -3.0f);

        static float angle = 0.0f; // Keep track of the angle
        angle += 0.5f;            // Increment the angle (adjust speed as needed)

        glRotatef(angle, 0.5f, 1.0f, 0.0f); // Use glRotatef for rotation

        // --- Drawing (Fixed-Function Pipeline) ---
        glBegin(GL_TRIANGLES);
        // Vertex 1 (Red, Bottom Left)
        glColor3f(1.0f, 0.0f, 0.0f);    // Set color to red
        glVertex3f(-0.5f, -0.5f, 0.0f);

        // Vertex 2 (Green, Bottom Right)
        glColor3f(0.0f, 1.0f, 0.0f);    // Set color to green
        glVertex3f(0.5f, -0.5f, 0.0f);

        // Vertex 3 (Blue, Top)
        glColor3f(0.0f, 0.0f, 1.0f);    // Set color to blue
        glVertex3f(0.0f, 0.5f, 0.0f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

#endif //FOO_H
