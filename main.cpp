#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <iostream>

#include "src/Cube.h"
#include "src/Point3.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    constexpr auto windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("Triangle", 640, 480, windowFlags);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Set render color to red
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


    // Clear the screen
    SDL_RenderClear(renderer);

    // Render the triangle.  This uses SDL_RenderGeometry for efficiency. You could alternatively use SDL_RenderDrawLine for simpler rendering.
    constexpr float scale = 50.0f;

    const auto cube = geometry::Cube(scale) + geometry::Point3{100, 100, 0};
    cube.draw(renderer);

    const auto cube2 = geometry::Cube(scale) + geometry::Point3{100, 300, 0};
    cube2.draw(renderer);

    // Update the screen
    SDL_RenderPresent(renderer);


    // Event loop (simplified for demonstration)
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while(SDL_PollEvent(&event) != 0)
        {
          if (event.type == SDL_EVENT_QUIT) {
            quit = true;
            break;
          }
        }
    }


    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
