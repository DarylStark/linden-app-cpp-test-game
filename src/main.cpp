#include <SDL2/SDL.h>
#include <linden_graphics/sdl2.h>
#include <linden_graphics/sdl2_texture.h>
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
        {1920, 1080});

    SDL_Window* window = w.get_sdl2_window_handle();

    // Create a texture
    linden::graphics::SDL2StreamingTexture t(*w.get_renderer(), {1920, 1080});
    SDL_Texture* texture = t.get_sdl2_texture_handle();

    // Add data to the texture
    t.lock();

    // Fill the texture with a simple pattern
    uint32_t* pixel_data = static_cast<uint32_t*>(t.get_pixels());
    for (int y = 0; y < 1080; ++y)
    {
        for (int x = 0; x < 1920; ++x)
        {
            t.set_pixel({x, y},
                        {static_cast<uint8_t>((x + y) % 256),
                         static_cast<uint8_t>((x * 2 + y * 2) % 256),
                         static_cast<uint8_t>((x * 3 + y * 3) % 256), 0xff});
        }
    }

    t.unlock();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    while (!quit)
    {
        linden::utils::ScopedTimer timer;
        linden::utils::FrameRateLimiter limiter(60);

        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Set background color
        SDL_SetRenderDrawColor(w.get_renderer()->get_sdl2_renderer_handle(), 0,
                               0x66, 0, 0xff);
        SDL_RenderClear(w.get_renderer()->get_sdl2_renderer_handle());

        // Render the texture
        SDL_RenderCopyEx(w.get_renderer()->get_sdl2_renderer_handle(), texture,
                         nullptr, nullptr, 0, nullptr, SDL_FLIP_NONE);

        // Render the texture
        SDL_RenderPresent(w.get_renderer()->get_sdl2_renderer_handle());
    }

    return 0;
}