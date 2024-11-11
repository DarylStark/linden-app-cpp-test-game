#include <SDL2/SDL.h>
#include <linden_graphics/sdl2.h>
#include <linden_graphics/sdl2_texture.h>
#include <linden_graphics/sdl2_window.h>
#include <linden_utils/frame_rate_limiter.h>
#include <linden_utils/scoped_timer.h>

#include <array>
#include <iostream>

int main()
{
    linden::graphics::SDL2 sdl2;

    // Create a window
    linden::graphics::SDL2Window w = sdl2.create_window(
        "SDL2 Basic Example", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        {1024 * 2, 1024 * 2});

    SDL_Window* window = w.get_sdl2_window_handle();

    // Load the texture for the background
    linden::graphics::SDL2ImageTexture t_bg(*w.get_renderer(), "assets/bg.jpg");
    SDL_Texture* texture_bg = t_bg.get_sdl2_texture_handle();

    // Load the texture for the car
    linden::graphics::SDL2ImageTexture t_image(*w.get_renderer(),
                                               "assets/car.png");
    SDL_Texture* texture_image = t_image.get_sdl2_texture_handle();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    SDL_Rect rect;
    rect.x = w.get_window_size().width / 2;
    rect.y = w.get_window_size().height / 2;
    rect.w = 348 / 3;
    rect.h = 685 / 3;
    double rot = 0;
    uint32_t speed = 15;
    uint8_t alpha = 255;
    uint32_t flash_speed = 500;

    // Define boolean flags for key states
    bool up_pressed = false;
    bool down_pressed = false;
    bool left_pressed = false;
    bool right_pressed = false;
    bool space_pressed = false;

    uint32_t current_texture = 0;

    auto start = std::chrono::high_resolution_clock::now();

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
                        if (!space_pressed)
                        {
                            space_pressed = true;
                            speed *= 3;
                            flash_speed /= 5;
                            std::cout << "DOWWWNNN" << std::endl;
                        }
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_MINUS:
                        if (alpha > 10)
                            alpha -= 10;
                        else
                            alpha = 0;
                        t_image.set_alpha(alpha);
                        break;
                    case SDLK_EQUALS:
                        if (alpha < 255 - 10)
                            alpha += 10;
                        else
                            alpha = 255;
                        t_image.set_alpha(alpha);
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
                        if (space_pressed)
                        {
                            speed /= 3;
                            flash_speed *= 5;
                            std::cout << "UP" << std::endl;
                            space_pressed = false;
                        }
                        break;
                }
            }
        }

        const auto now = std::chrono::high_resolution_clock::now();
        const auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        if (elapsed.count() > flash_speed)
        {
            current_texture++;
            start = now;
        }

        // Update position and rotation based on key states
        if (up_pressed)
        {
            const int new_x = rect.x + (speed * cos((rot - 90) * M_PI / 180.0));
            const int new_y = rect.y + (speed * sin((rot - 90) * M_PI / 180.0));
            rect.x = new_x;
            rect.y = new_y;
        }
        if (down_pressed)
        {
            const int new_x = rect.x - (speed * cos((rot - 90) * M_PI / 180.0));
            const int new_y = rect.y - (speed * sin((rot - 90) * M_PI / 180.0));
            rect.x = new_x;
            rect.y = new_y;
        }
        if (left_pressed)
        {
            rot -= 5;
        }
        if (right_pressed)
        {
            rot += 5;
        }

        // Clear screen
        w.get_renderer()->clear();

        // Set background
        w.get_renderer()->render_texture(
            t_bg, {{0, 0},
                   {w.get_window_size().width, w.get_window_size().height},
                   0,
                   {0, 0},
                   false,
                   false});

        // Render the texture
        w.get_renderer()->render_texture(t_image, {{rect.x, rect.y},
                                                   {rect.w, rect.h},
                                                   rot,
                                                   {rect.w / 2, rect.h / 2},
                                                   false,
                                                   false});

        // Render the texture
        w.get_renderer()->render();
    }

    return 0;
}