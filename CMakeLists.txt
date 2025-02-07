cmake_minimum_required(VERSION 3.30)
project(chunk_preview)
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(libs/SDL EXCLUDE_FROM_ALL)

set(SDLIMAGE_VENDORED ON)
set(SDLIMAGE_AVIF OFF)	# disable formats we don't use to make the build faster and smaller.
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_WEBP OFF)
add_subdirectory(libs/SDL_image EXCLUDE_FROM_ALL)


## Mixer for audio
add_subdirectory(libs/SDL_mixer EXCLUDE_FROM_ALL)

set(SDLTTF_VENDORED OFF)
add_subdirectory(libs/SDL_ttf EXCLUDE_FROM_ALL)

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
        src/Quaternion.h
        src/Point2.h
        src/Point3.h
        src/ProjectionHelper.h)

# Link ImGui to the project
target_include_directories(chunk_preview PRIVATE ${IMGUI_DIR})
target_link_libraries(chunk_preview PRIVATE imgui)

#target_link_libraries(chunk_preview PRIVATE SDL3::SDL3)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        SDL3_image::SDL3_image
        SDL3::SDL3
)

