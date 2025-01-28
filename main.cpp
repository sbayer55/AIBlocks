#include <SDL3/SDL.h>  // Include SDL3 header
#include <iostream>
#include <vector>
#include <cmath>

// ... (Point3D struct, rotateY, and project functions remain the same)

void renderCube(SDL_Renderer* renderer, float angle) {
    // ... (Cube vertices and edges definitions remain the same)

    // Drawing with SDL3 (API change here)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (const auto& edge : edges) {
        SDL_RenderDrawLineF(renderer, // Use DrawLineF for floating-point coordinates
                           vertices[edge.first].x,
                           vertices[edge.first].y,
                           vertices[edge.second].x,
                           vertices[edge.second].y);
    }
}


int main(int argc, char* argv[]) {
    // SDL3 initialization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Window creation (SDL3 API change)
    SDL_Window* window = SDL_CreateWindow("Chunk Preview",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) { /* Error handling */ }


    // Renderer creation (SDL3 API change)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Add VSYNC for smoother animation
    if (!renderer) { /* Error handling */ }


    float angle = 0.0f;
    bool running = true;
    while (running) {
         // ... (Event handling remains the same)

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderCube(renderer, angle);

        SDL_RenderPresent(renderer); // No need to pass the window in SDL3
        angle += 1.0f;
    }

    // SDL3 cleanup (API change)
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); // Important: Shut down SDL subsystems properly

    return 0;
}