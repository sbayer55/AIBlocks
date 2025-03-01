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

inline void showFullscreenTest() {
        // Create a fullscreen window that should be impossible to miss
        const ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        if (ImGui::Begin("FULLSCREEN TEST", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar)) {

            ImGui::Text("If you can see this, ImGui is working!");
            ImGui::Text("Display Size: %.1f x %.1f", io.DisplaySize.x, io.DisplaySize.y);
            ImGui::Text("Framebuffer Scale: %.1f x %.1f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
            ImGui::Text("OpenGL: %s", glGetString(GL_VERSION));

            ImGui::Separator();

            static float col[3] = { 1.0f, 0.0f, 0.0f };
            ImGui::ColorEdit3("Background", col);

            ImGui::Button("This is a button");
        }
        ImGui::End();
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
        // Save current OpenGL state
        GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
        GLint last_tex; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_tex);
        GLboolean depth_test = glIsEnabled(GL_DEPTH_TEST);
        GLboolean blend = glIsEnabled(GL_BLEND);

        // Configure OpenGL state for ImGui rendering
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        // Get current framebuffer size
        int display_w, display_h;
        glfwGetFramebufferSize(g_Window, &display_w, &display_h);

        // Set viewport to cover the entire framebuffer
        glViewport(0, 0, display_w, display_h);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        // Restore OpenGL state
        glBindTexture(GL_TEXTURE_2D, last_tex);
        glViewport(last_viewport[0], last_viewport[1], last_viewport[2], last_viewport[3]);
        if (depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
        if (!blend) glDisable(GL_BLEND);
    }

    // Display transform information as an overlay
    inline void displayTransformOverlay(const GameObject& gameObject) {
        const auto& transform = gameObject.get_transform();

        // Create a window in the top-left corner with camera transform info
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.7f); // Semi-transparent background

        if (ImGui::Begin("Camera Transform", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            ImGui::Text("Position:");
            ImGui::Text("  X: %.2f", transform.position.x);
            ImGui::Text("  Y: %.2f", transform.position.y);
            ImGui::Text("  Z: %.2f", transform.position.z);

            ImGui::Separator();

            ImGui::Text("Rotation (Quaternion):");
            ImGui::Text("  W: %.2f", transform.rotation.w);
            ImGui::Text("  X: %.2f", transform.rotation.x);
            ImGui::Text("  Y: %.2f", transform.rotation.y);
            ImGui::Text("  Z: %.2f", transform.rotation.z);

            // Convert quaternion to Euler angles for easier reading
            const float pitch = glm::degrees(atan2(2.0f * (transform.rotation.y * transform.rotation.z + transform.rotation.w * transform.rotation.x),
                                                  transform.rotation.w * transform.rotation.w - transform.rotation.x * transform.rotation.x - transform.rotation.y * transform.rotation.y + transform.rotation.z * transform.rotation.z));
            const float yaw = glm::degrees(asin(-2.0f * (transform.rotation.x * transform.rotation.z - transform.rotation.w * transform.rotation.y)));
            const float roll = glm::degrees(atan2(2.0f * (transform.rotation.x * transform.rotation.y + transform.rotation.w * transform.rotation.z),
                                                 transform.rotation.w * transform.rotation.w + transform.rotation.x * transform.rotation.x - transform.rotation.y * transform.rotation.y - transform.rotation.z * transform.rotation.z));

            ImGui::Separator();

            ImGui::Text("Euler Angles (degrees):");
            ImGui::Text("  Pitch: %.2f", pitch);
            ImGui::Text("  Yaw: %.2f", yaw);
            ImGui::Text("  Roll: %.2f", roll);
        }
        ImGui::End();
    }
}

#endif // IMGUI_IMPLEMENTATION_H