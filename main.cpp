#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/gtx/io.hpp"
#include "src/Cube.h"
#include "src/Foo.h"
#include "src/MouseKeyboardMovementComponent.h"
#include "src/OpenGLDebug.h"
#include "src/Vertex.h"
#include "imgui.h"
#include "src/ImguiImplementation.h"

using GLFWWindowPtr = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;

void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void centerCursor(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Divide by 4 for retina display because of how macOS reports the size
    glfwSetCursorPos(window, width / 4.0, height / 4.0);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    std::cout << "Window resized: " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}

void setupPerspectiveProjection(GLFWwindow* window, const GLfloat fov, const GLfloat nearPlane, const GLfloat farPlane) {
    // int width, height;
    // glfwGetFramebufferSize(window, &width, &height);
    constexpr int width = 640;
    constexpr int height = 480;

    // Calculate aspect ratio
    constexpr GLfloat aspectRatio = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);

    // Convert fov from degrees to radians
    const GLfloat fovRadians = fov / 2.0f * static_cast<GLfloat>(M_PI) / 180.0f;

    const GLfloat sine = sin(fovRadians);
    const GLfloat deltaZ = farPlane - nearPlane;

    if (deltaZ == 0 || sine == 0 || aspectRatio == 0){
        std::cerr << "Invalid projection parameters" << std::endl;
        return;
    }

    const GLfloat cotangent = cos(fovRadians) / sine;

    const float top = nearPlane * std::tan(fovRadians / 2.0f);
    const float right = top * aspectRatio;
    const float left = -right;
    const float bottom = -top;

    std::cout << "Projection Top: " << top
            << " Right: " << right
            << " Left: " << left
            << " Bottom: " << bottom
            << std::endl;

    // Construct the perspective projection matrix
    const float projectionMatrix[16] = {
        cotangent / aspectRatio, 0.0f, 0.0f, 0.0f,
        0.0f, cotangent, 0.0f, 0.0f,
        // 0.0f, 0.0f, -(farPlane + nearPlane) / deltaZ, -1.0f,
        0.0f, 0.0f, -(farPlane + nearPlane) / deltaZ, 2 * farPlane * nearPlane / deltaZ,
        0.0f, 0.0f, -2.0f * farPlane * nearPlane / deltaZ, 0.0f
    };

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(projectionMatrix);
}

auto initializeOpenGL(const int windowWidth, const int windowHeight) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return GLFWWindowPtr{nullptr, glfwDestroyWindow};
    }
    // glfwWindowHint(GLFW_SAMPLES, 4); // Multi-sampling for antialiasing
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Allow window resizing

    // Set error callback
    glfwSetErrorCallback(errorCallback);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Chunk Preview", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return GLFWWindowPtr{nullptr, glfwDestroyWindow};
    }
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Enable vsync
    glfwSwapInterval(1);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Hide the cursor
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Solid

    return GLFWWindowPtr(window, glfwDestroyWindow);
}

int main(int argc, char* argv[]) {
    constexpr int windowWidth = 1200;
    constexpr int windowHeight = 800;
    GLFWWindowPtr window = initializeOpenGL(windowWidth, windowHeight);
    if (!window) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return 1;
    }

    ui::initImGui(window.get());

    // Print OpenGL version info for debugging
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;

    constexpr float scale = 10.0f;
    constexpr float zOffset = 50.0f;
    const std::array cubes = {
        Cube(Transform::of({windowWidth / 2, windowHeight / 2, 1000.0f}, 1000)),
        Cube(Transform::of({windowWidth / 2, windowHeight / 2, 1000.0f}, 100)),
        Cube(Transform::of({windowWidth / 2, windowHeight / 2, 100.0f}, 10)),
        Cube(Transform::of({windowWidth / 2, windowHeight / 2, 10.0f}, 1)),
        Cube(Transform::of({0, 0, zOffset}, scale)),
        Cube(Transform::of({0, windowWidth, zOffset}, scale)),
        Cube(Transform::of({windowHeight, 0, zOffset}, scale)),
        Cube(Transform::of({windowHeight, windowWidth, zOffset}, scale)),
        Cube(Transform::of({50, 50, zOffset}, scale)),
        Cube(Transform::of({50, 200, zOffset}, scale)),
        Cube(Transform::of({200, 50, zOffset}, scale)),
        Cube(Transform::of({200, 200, zOffset}, scale)),
    };

    GameObject camera{};
    const auto movement_component = camera.add_component<MouseKeyboardMovementComponent>();
    glfwSetWindowUserPointer(window.get(), movement_component);
    glfwSetKeyCallback(window.get(), [](GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
        // Pass events to ImGui
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

        // Check if ImGui wants to capture keyboard
        if (ImGui::GetIO().WantCaptureKeyboard) {
            return;
        }

        // Handle application key events
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        auto* component = static_cast<MouseKeyboardMovementComponent*>(glfwGetWindowUserPointer(window));
        component->on_key_event(window, key, scancode, action, mods);
    });
    glfwSetMouseButtonCallback(window.get(), ImGui_ImplGlfw_MouseButtonCallback);
    glfwSetScrollCallback(window.get(), ImGui_ImplGlfw_ScrollCallback);
    glfwSetCursorPosCallback(window.get(), [](GLFWwindow* window, const double xpos, const double ypos) {
        // Pass events to ImGui
        // ImGui_ImplGlfw_CursorPosCallback is not available by default, so we'll handle it manually

        // Check if ImGui wants to capture mouse
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }

        auto* component = static_cast<MouseKeyboardMovementComponent*>(glfwGetWindowUserPointer(window));
        component->on_mouse_move(window, xpos, ypos);
    });
    glfwSetCharCallback(window.get(), ImGui_ImplGlfw_CharCallback);

    // Main loop
    while (!glfwWindowShouldClose(window.get())) {
        // Clear the view
        glClearColor(0.6f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ui::beginFrame();

        // // Setup Projection
        // int framebufferWidth, framebufferHeight;
        // glfwGetFramebufferSize(window.get(), &framebufferWidth, &framebufferHeight);
        // glViewport(0, 0, framebufferWidth, framebufferHeight);
        //
        // glMatrixMode(GL_PROJECTION);
        // const float aspectRatio = static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);
        // glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
        // glLoadMatrixf(value_ptr(projection));
        //
        // // Objects
        // glMatrixMode(GL_MODELVIEW);
        // glLoadIdentity();
        //
        // glm::mat4 cameraRotation = toMat4(camera.get_transform().rotation);
        //
        // // Create a rotation matrix around the X-axis
        // glm::mat4 view = translate(glm::mat4(1.0f), camera.get_transform().position);
        // view = cameraRotation * view;
        // glLoadMatrixf(value_ptr(view));
        //
        // // debug::debugViewport();
        // // debug::debugMatrices();
        // debug::drawDebugAxes();
        // debug::drawBoundingBox();
        //
        // for (auto& cube : cubes) {
        //     glPushMatrix();
        //     cube.draw();
        //     glPopMatrix();
        // }

        // glBegin(GL_TRIANGLES);
        // glColor3f(1.0f, 1.0f, 1.0f);  // White color
        // glVertex3f(0.0f,  100.0f, 1.0f);  // Top vertex
        // glVertex3f(100.0f, 100.0f, 10.0f);  // Bottom-left vertex
        // glVertex3f(0.0f, 0.0f, 10.0f);  // Bottom-right vertex
        // glEnd();

        debug::checkGLErrors("Main Loop - Before ImGui");

        // ui::displayTransformOverlay(camera);

        ui::showFullscreenTest();

        // // Add a simple test window to check if ImGui is working at all
        // ImGui::Begin("ImGui Test Window");
        // ImGui::Text("Hello from ImGui!");
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        // if (ImGui::Button("Click Me")) {
        //     std::cout << "Button clicked!" << std::endl;
        // }
        // ImGui::End();
        //
        // // Add ImGui demo window to test full ImGui functionality
        // static bool show_demo_window = true;
        // ImGui::ShowDemoWindow(&show_demo_window);

        ui::render();

        debug::checkGLErrors("Main Loop - After ImGui");

        // vsync
        glfwSwapBuffers(window.get());
        // check for keypress, mouse, window resize, errors
        glfwPollEvents();

        // if (!ImGui::GetIO().WantCaptureMouse) {
        //     centerCursor(window.get());
        // }
    }

    ui::shutdownImGui();

    // Cleanup
    const decltype(&glfwDestroyWindow)& del = window.get_deleter();
    auto* released = window.release();
    del(released);

    glfwTerminate();

    return 0;
}
