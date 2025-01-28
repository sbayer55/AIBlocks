cmake_minimum_required(VERSION 3.30)
project(chunk_preview)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL3 REQUIRED) # Find SDL3 package

#set(SDL3_DIR /libs/SDL/src)
#set(SDL_FILES
#        ${SDL3_DIR}/SDL.c
#)
#add_library(SDL3 STATIC ${SDL_FILES})

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
add_executable(chunk_preview main.cpp)

# Link ImGui to the project
target_include_directories(chunk_preview PRIVATE ${IMGUI_DIR})
target_link_libraries(chunk_preview PRIVATE imgui)

#target_include_directories(chunk_preview PRIVATE ${SDL3_DIR})
target_link_libraries(chunk_preview PRIVATE SDL3::SDL3)

