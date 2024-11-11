#include <SDL2/SDL.h>
#include <linden_graphics/sdl2.h>
#include <linden_graphics/sdl2_texture.h>
#include <linden_graphics/sdl2_window.h>
#include <linden_utils/frame_rate_limiter.h>
#include <linden_utils/scoped_timer.h>

#include <array>
#include <iostream>
#include <map>

int main()
{
    linden::graphics::SDL2 sdl2;

    // Create a window
    linden::graphics::SDL2Window w = sdl2.create_window(
        "SDL2 Basic Example", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        {1920, 1080});

    // Load foliage texture
    linden::graphics::SDL2ImageTexture foliage_texture(*w.get_renderer(),
                                                       "assets/foliage.png");
    std::map<std::string, std::shared_ptr<linden::graphics::SDL2Texture>>
        textures;
    textures["tree1"] =
        foliage_texture.create_sub_texture({478, 0}, {102, 287});
    textures["car"] = std::make_shared<linden::graphics::SDL2ImageTexture>(
        *w.get_renderer(), "assets/car.png");

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    int32_t foliage_start_x = 100;

    linden::graphics::Position rotate_center_accelerate = {
        0, textures["car"]->get_size().height - 1};
    linden::graphics::Position rotate_center_deaccelerate = {
        textures["car"]->get_size().width - 1,
        textures["car"]->get_size().height - 1};

    linden::graphics::TextureRenderOptions car_options = {
        .position = {50, 780},
        .rotation_center = rotate_center_accelerate,
        .scale = 8};
    int32_t speed = 0;

    while (!quit)
    {
        linden::utils::ScopedTimer timer;
        linden::utils::FrameRateLimiter limiter(60);

        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        if (speed > 0)
                        {
                            car_options.angle = 3;
                            car_options.rotation_center =
                                rotate_center_deaccelerate;
                            speed -= 5;
                            if (speed < 0) speed = 0;
                        }
                        break;
                    case SDLK_RIGHT:
                        speed += 5;
                        car_options.rotation_center = rotate_center_accelerate;
                        if (speed > 180) speed = 180;
                        car_options.angle = -3;
                        break;
                    default:
                        break;
                }
            }
            if (e.type == SDL_KEYUP)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_RIGHT:
                    case SDLK_LEFT:
                        car_options.angle = 0;
                        break;
                }
            }
        }

        foliage_start_x -= speed;
        if (foliage_start_x > 100)
        {
            foliage_start_x = 100;
            speed = 0;
        }
        std::cout << speed;

        // Clear screen
        w.get_renderer()->clear({0, 0x44, 0, 0xff});

        // Render the foliage
        w.get_renderer()->render_texture(*textures.at("tree1"),
                                         {.position = {foliage_start_x, 500}});
        w.get_renderer()->render_texture(
            *textures.at("tree1"), {.position = {foliage_start_x + 152, 480}});
        w.get_renderer()->render_texture(
            *textures.at("tree1"), {.position = {foliage_start_x + 302, 520}});

        // Render the car
        w.get_renderer()->render_texture(*textures.at("car"), car_options);

        // Render the texture
        w.get_renderer()->render();
    }

    return 0;
}