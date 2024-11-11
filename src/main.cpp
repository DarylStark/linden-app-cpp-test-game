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

    // Load the texture for the car
    linden::graphics::SDL2ImageTexture t_image(*w.get_renderer(),
                                               "assets/car.png");

    // Main loop flag
    bool quit = false;

    // Main character render options
    linden::graphics::TextureRenderOptions car_render_options = {
        .position = {w.get_window_size().width / 4,
                     w.get_window_size().height / 4},
        .size = {348 / 3, 685 / 3},
        .angle = 0};

    uint32_t speed = 15;

    // Define boolean flags for key states
    bool up_pressed = false;
    bool down_pressed = false;
    bool left_pressed = false;
    bool right_pressed = false;
    bool space_pressed = false;

    uint32_t current_texture = 0;
    uint8_t alpha = 0xff;

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
                            std::cout << "UP" << std::endl;
                            space_pressed = false;
                        }
                        break;
                }
            }
        }

        // Update position and rotation based on key states
        if (up_pressed)
        {
            const int new_x =
                car_render_options.position.x +
                (speed * cos((car_render_options.angle - 90) * M_PI / 180.0));
            const int new_y =
                car_render_options.position.y +
                (speed * sin((car_render_options.angle - 90) * M_PI / 180.0));
            car_render_options.position.x = new_x;
            car_render_options.position.y = new_y;
        }
        if (down_pressed)
        {
            const int new_x =
                car_render_options.position.x -
                (speed * cos((car_render_options.angle - 90) * M_PI / 180.0));
            const int new_y =
                car_render_options.position.y -
                (speed * sin((car_render_options.angle - 90) * M_PI / 180.0));
            car_render_options.position.x = new_x;
            car_render_options.position.y = new_y;
        }
        if (left_pressed)
        {
            car_render_options.angle -= 9;
        }
        if (right_pressed)
        {
            car_render_options.angle += 9;
        }

        // Clear screen
        w.get_renderer()->clear();

        // Set background
        w.get_renderer()->render_texture(t_bg);

        // Render the texture
        w.get_renderer()->render_texture(t_image, car_render_options);

        // Render the texture
        w.get_renderer()->render();
    }

    return 0;
}