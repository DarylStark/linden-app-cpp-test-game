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
    linden::graphics::SDL2Window w =
        sdl2.create_window("SDL2 Basic Example", {300, 300}, {600, 600});

    SDL_Window* window = w.get_window_handle();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    SDL_SetRenderDrawColor(w.get_renderer_handle(), 0, 0x66, 0, 0xff);
    SDL_RenderClear(w.get_renderer_handle());

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

        // Update screen
        SDL_RenderPresent(w.get_renderer_handle());
    }

    return 0;
}