#ifndef IMGUI_IMPLEMENTATION_H
#define IMGUI_IMPLEMENTATION_H

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#include "GLFW/glfw3.h"
#include "GameObject.h"
#include "Transform.h"
#include <string>

namespace ui {
    inline GLFWwindow* g_Window;

    // Initialize ImGui for use with GLFW and OpenGL
    inline void initImGui(GLFWwindow* window) {
        g_Window = window;
        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL2_Init();

        // Setup ImGui style
        ImGui::StyleColorsDark();

        // macOS specific settings for Retina displays
#ifdef __APPLE__
        // Make fonts larger for better visibility on Retina
        ImFontConfig config;
        config.SizePixels = 16.0f;
        io.Fonts->AddFontDefault(&config);
        std::cout << "Using larger font size for macOS" << std::endl;
#endif

        std::cout << "ImGui initialized with version: " << IMGUI_VERSION << std::endl;
    }

    // Cleanup ImGui resources
    inline void shutdownImGui() {
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // Begin a new ImGui frame
    inline void beginFrame() {
        // Make sure ImGui knows about the correct display size
        ImGuiIO& io = ImGui::GetIO();
        int display_w, display_h;
        int window_w, window_h;
        glfwGetFramebufferSize(g_Window, &display_w, &display_h);
        glfwGetWindowSize(g_Window, &window_w, &window_h);
        io.DisplaySize = ImVec2((float)window_w, (float)window_h);

        // For retina displays, set the correct scaling
        if (window_w > 0 && window_h > 0)
            io.DisplayFramebufferScale = ImVec2((float)display_w / window_w, (float)display_h / window_h);

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    // Render ImGui data
    inline void render() {
        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    // Display transform information as an overlay
    inline void displayTransformOverlay(const GameObject& gameObject) {
        const auto& transform = gameObject.get_transform();

        // Create a window in the top-left corner with camera transform info
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        // ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 600));
        ImGui::SetNextWindowBgAlpha(0.7f); // Semi-transparent background

        if (ImGui::Begin("Camera Transform")) {
            ImGui::Text("Position:");
            ImGui::Text("  X: %.2f", transform.position.x);
            ImGui::Text("  Y: %.2f", transform.position.y);
            ImGui::Text("  Z: %.2f", transform.position.z);

            ImGui::Separator();

            auto rotation = transform.rotation;

            ImGui::Text("Rotation (Quaternion):");
            ImGui::Text("  W: %.2f", rotation.w);
            ImGui::Text("  X: %.2f", rotation.x);
            ImGui::Text("  Y: %.2f", rotation.y);
            ImGui::Text("  Z: %.2f", rotation.z);

            ImGui::Separator();

            glm::vec3 eulerAngles = glm::eulerAngles(rotation);
            eulerAngles = glm::degrees(eulerAngles);

            ImGui::Text("Euler Angles (degrees):");
            ImGui::Text("  Pitch (Up/Down): %.2f", eulerAngles.x);
            ImGui::Text("  Yaw (Left/Right): %.2f", eulerAngles.y);
            ImGui::Text("  Roll: %.2f", eulerAngles.z);
        }
        ImGui::End();
    }
}

#endif // IMGUI_IMPLEMENTATION_H