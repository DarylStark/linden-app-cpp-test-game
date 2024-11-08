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
        {1280, 1024});

    SDL_Window* window = w.get_sdl2_window_handle();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    SDL_SetRenderDrawColor(w.get_renderer()->get_sdl2_renderer_handle(), 0,
                           0x66, 0, 0xff);
    SDL_RenderClear(w.get_renderer()->get_sdl2_renderer_handle());
    SDL_RenderPresent(w.get_renderer()->get_sdl2_renderer_handle());

    // Get information about the window
    linden::graphics::Size window_size = w.get_window_size();
    linden::graphics::Size drawable_size = w.get_drawable_size();
    std::cout << "Window size: " << window_size.width << "x"
              << window_size.height << std::endl;
    std::cout << "Drawable size: " << drawable_size.width << "x"
              << drawable_size.height << std::endl;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        linden::graphics::Position window_position = w.get_window_position();
        std::cout << "Window position: " << window_position.x << "x"
                  << window_position.y << std::endl;
        SDL_Delay(10);
    }

    return 0;
}