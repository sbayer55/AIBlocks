cmake_minimum_required(VERSION 3.30)
project(chunk_preview)
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8  # You can update this to a newer version if needed
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8  # You can update this to a newer version if needed
)

# Configure GLFW build options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)

# Add ImGui files
set(IMGUI_DIR libs/imgui)
set(IMGUI_FILES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
)

# Add ImGui to the project
add_library(imgui STATIC ${IMGUI_FILES})
add_executable(chunk_preview main.cpp
        src/Cube.h
        src/Vertex.h
        src/Transform.h
        src/OpenGLDebug.h
        src/Foo.h
        src/GameObject.h
        src/MouseKeyboardMovementComponent.h
)

# Link ImGui to the project
target_include_directories(chunk_preview PRIVATE ${IMGUI_DIR})

# Link GLFW and OpenGL

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        glfw
        OpenGL::GL
        glm
        imgui
)

# On macOS, we need to link additional frameworks
if(APPLE)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

