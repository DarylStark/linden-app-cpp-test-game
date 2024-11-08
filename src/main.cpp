#include <SDL2/SDL.h>
#include <linden_graphics/sdl2.h>
#include <linden_graphics/sdl2_window.h>
#include <linden_utils/frame_rate_limiter.h>
#include <linden_utils/scoped_timer.h>

#include <iostream>

int main()
{
    linden::graphics::SDL2 sdl2;

    // Create a window
    linden::graphics::SDL2Window w = sdl2.create_window(
        "SDL2 Basic Example", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        {1024, 768});

    SDL_Window* window = w.get_window_handle();

    // Create a renderer
    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    while (!quit)
    {
        linden::utils::ScopedTimer scopedTimer;
        linden::utils::FrameRateLimiter frameRateLimiter(60);

        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x66, 0x00, 0xff);
        SDL_RenderClear(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}