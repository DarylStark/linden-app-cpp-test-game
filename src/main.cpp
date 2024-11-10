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
    linden::graphics::SDL2StreamingTexture t(*w.get_renderer(), {7, 12});
    SDL_Texture* texture = t.get_sdl2_texture_handle();

    // Add data to the texture
    t.lock();

    // Fill the texture with a simple pattern
    for (int32_t x = 0; x < 7; x++)
    {
        for (int32_t y = 0; y < 12; y++)
        {
            t.set_pixel({x, y}, {255, 255, 255, 0});
        }
    }

    t.set_pixel({0, 2}, {0, 0, 0, 255});
    t.set_pixel({6, 2}, {0, 0, 0, 255});
    t.set_pixel({0, 3}, {0, 0, 0, 255});
    t.set_pixel({6, 3}, {0, 0, 0, 255});
    t.set_pixel({0, 8}, {0, 0, 0, 255});
    t.set_pixel({6, 8}, {0, 0, 0, 255});
    t.set_pixel({0, 9}, {0, 0, 0, 255});
    t.set_pixel({6, 9}, {0, 0, 0, 255});

    for (int32_t y = 0; y <= 11; y++)
    {
        t.set_pixel({1, y}, {0, 0, 0, 255});
        t.set_pixel({2, y}, {0, 0, 0, 255});
        t.set_pixel({3, y}, {0, 0, 0, 255});
        t.set_pixel({4, y}, {0, 0, 0, 255});
        t.set_pixel({5, y}, {0, 0, 0, 255});
    }

    t.set_pixel({1, 0}, {255, 255, 255, 255});
    t.set_pixel({5, 0}, {255, 255, 255, 255});

    t.unlock();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 70;
    rect.h = 120;
    int32_t rot = 0;
    uint32_t speed = 10;

    // Define boolean flags for key states
    bool up_pressed = false;
    bool down_pressed = false;
    bool left_pressed = false;
    bool right_pressed = false;

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
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_UP:
                        up_pressed = true;
                        break;
                    case SDLK_DOWN:
                        down_pressed = true;
                        break;
                    case SDLK_LEFT:
                        left_pressed = true;
                        break;
                    case SDLK_RIGHT:
                        right_pressed = true;
                        break;
                    case SDLK_SPACE:
                        speed *= 3;
                        std::cout << "Speed: " << speed << std::endl;
                        break;
                }
            }
            // Key release events
            else if (e.type == SDL_KEYUP)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_UP:
                        up_pressed = false;
                        break;
                    case SDLK_DOWN:
                        down_pressed = false;
                        break;
                    case SDLK_LEFT:
                        left_pressed = false;
                        break;
                    case SDLK_RIGHT:
                        right_pressed = false;
                        break;
                    case SDLK_SPACE:
                        speed /= 3;
                        std::cout << "Speed: " << speed << std::endl;
                        break;
                }
            }
        }

        // Update position and rotation based on key states
        if (up_pressed)
        {
            const int new_x = rect.x + (speed * cos((rot - 90) * M_PI / 180.0));
            const int new_y = rect.y + (speed * sin((rot - 90) * M_PI / 180.0));
            if (new_x >= 0 && new_x <= 1920 - 70 && new_y >= 0 &&
                new_y <= 1080 - 120)
            {
                rect.x = new_x;
                rect.y = new_y;
            }
        }
        if (down_pressed)
        {
            const int new_x = rect.x - (speed * cos((rot - 90) * M_PI / 180.0));
            const int new_y = rect.y - (speed * sin((rot - 90) * M_PI / 180.0));
            if (new_x >= 0 && new_x <= 1920 - 70 && new_y >= 0 &&
                new_y <= 1080 - 120)
            {
                rect.x = new_x;
                rect.y = new_y;
            }
        }
        if (left_pressed)
        {
            rot -= 5;
        }
        if (right_pressed)
        {
            rot += 5;
        }

        // Set background color
        SDL_SetRenderDrawColor(w.get_renderer()->get_sdl2_renderer_handle(),
                               0xcc, 0xcc, 0xcc, 0xff);
        SDL_RenderClear(w.get_renderer()->get_sdl2_renderer_handle());

        // Render the texture
        SDL_RenderCopyEx(w.get_renderer()->get_sdl2_renderer_handle(), texture,
                         nullptr, &rect, rot, nullptr, SDL_FLIP_NONE);

        // Render the texture
        SDL_RenderPresent(w.get_renderer()->get_sdl2_renderer_handle());
    }

    return 0;
}