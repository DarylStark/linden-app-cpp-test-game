

#ifndef OLD_CODE

#include <linden_graphics/sdl2_eventbus.h>

#include "linden/sdl2/context.h"
#include "linden/sdl2/image_sprite.h"
#include "linden/utils/frame_rate_limiter.h"
#include "linden/utils/scoped_timer.h"

int main()
{
    // Context
    linden::sdl2::Context context;

    // Window
    auto window = context.create_window(
        "Hello World", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        {1920, 1080});

    // Image sprite
    linden::sdl2::ImageSprite image_sprite(window.get_renderer(),
                                           "assets/car.png");

    // Event Bus (OLD CODE)
    bool quit = false;
    linden::graphics::SDL2EventBus event_bus;
    event_bus.on_window_close([&quit](const SDL_Event&) { quit = true; });
    uint16_t x = 0;

    while (!quit)
    {
        linden::utils::ScopedTimer timer;
        linden::utils::FrameRateLimiter limiter(120);

        event_bus.handle_sdl_events();

        window.get_renderer().set_draw_color({128, 64, 0, 128});
        window.get_renderer().clear();

        image_sprite.render({
            .destination = {.position = {0, 0}, .size = {1920 / 2, 1080 / 2}},
            .source = {.position = {0, 0}, .size = {200, 200}},
            .rotation = {.angle = (x += 5) % 360,
                         .center = {1920 / 4, 1080 / 4},
                         .flip_horizontal = false,
                         .flip_vertical = false},
        });

        window.get_renderer().present();
    }

    return 0;
}

#endif

#ifdef OLD_CODE

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <linden/utils/frame_rate_limiter.h>
#include <linden/utils/scoped_timer.h>
#include <linden_graphics/sdl2.h>
#include <linden_graphics/sdl2_eventbus.h>
#include <linden_graphics/sdl2_font.h>
#include <linden_graphics/sdl2_image_texture.h>
#include <linden_graphics/sdl2_target_texture.h>
#include <linden_graphics/sdl2_text_texture.h>
#include <linden_graphics/sdl2_texture.h>
#include <linden_graphics/sdl2_window.h>

#include <array>
#include <iostream>
#include <map>

#include "level_creator/background_generator.h"
#include "level_creator/level_data.h"
#include "resource_manager/texture_manager.h"
#include "tile_grid/tile_grid.h"

linden::test_game::LevelData level_1{.length_in_meters = 5000};
int main()
{
    linden::graphics::SDL2 sdl2;

    // Create a window
    linden::graphics::SDL2Window w = sdl2.create_window(
        "SDL2 Basic Example", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        {1920, 1080});

    linden::test_game::TextureManager folioage(*w.get_renderer());
    linden::test_game::TextureManager game_assets(*w.get_renderer());

    linden::test_game::TileGrid tg(*w.get_renderer(), {1920, 1080}, {256, 256});

    // Load the texture for the car
    game_assets.add_texture_from_file("car", "assets/car.png");

    // Load the texture for the foliage
    folioage.add_texture_from_file("foliage", "assets/foliage.png");
    folioage.add_texture_from_spritesheet("foliage", "tree_04", {478, 0},
                                          {102, 287});
    folioage.add_texture_from_spritesheet("foliage", "tree_10", {0, 403},
                                          {151, 211});

    // Create the level texture creator
    linden::test_game::BackgroundGenerator ltc(w.get_renderer(), folioage,
                                               level_1);

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    int32_t level_start_x = 100;

    linden::graphics::Position rotate_center_accelerate = {
        0, game_assets.get("car")->get_size().height - 1};
    linden::graphics::Position rotate_center_deaccelerate = {
        game_assets.get("car")->get_size().width - 1,
        game_assets.get("car")->get_size().height - 1};

    linden::graphics::TextureRenderOptions car_options = {
        .position = {50, 830},
        .rotation_center = rotate_center_accelerate,
        .scale = 8};
    int32_t speed = 0;
    uint8_t lane = 0;
    bool in_rear = false;

    // Eventbus
    linden::graphics::SDL2EventBus event_bus;
    const linden::graphics::SDL2EventCallback left_down =
        [&speed, &car_options, &rotate_center_deaccelerate](const SDL_Event& e)
    {
        std::cout << "Left down" << std::endl;
        if (speed > 0)
        {
            car_options.angle = 3;
            car_options.rotation_center = rotate_center_deaccelerate;
            speed -= 5;
            if (speed < 0) speed = 0;
        }
    };
    const linden::graphics::SDL2EventCallback right_down =
        [&speed, &car_options, &rotate_center_accelerate](const SDL_Event& e)
    {
        std::cout << "Right down" << std::endl;
        speed += 1;
        car_options.rotation_center = rotate_center_accelerate;
        if (speed > 180) speed = 180;
        car_options.angle = -3;
    };
    const linden::graphics::SDL2EventCallback up =
        [&car_options](const SDL_Event& e) { car_options.angle = 0; };
    const auto change_lane = [&lane, &speed](const SDL_Event& e)
    {
        if (speed == 0) return;
        if (lane == 0)
            lane = 1;
        else
            lane = 0;
    };
    event_bus.on_specific_key_down(SDLK_LEFT, left_down);
    event_bus.on_specific_key_down(SDLK_RIGHT, right_down);
    event_bus.on_specific_key_up(SDLK_LEFT, up);
    event_bus.on_specific_key_up(SDLK_RIGHT, up);
    event_bus.on_specific_key_up(SDLK_UP, change_lane);
    event_bus.on_specific_key_up(SDLK_DOWN, change_lane);

    event_bus.on_window_close([&quit](const SDL_Event&) { quit = true; });

    // Text rendering
    linden::graphics::SDL2Font l_font("assets/digital-7.regular.ttf", 48);
    linden::graphics::SDL2TextTexture text_texture(
        *w.get_renderer(), "Hello", l_font, {255, 255, 255, 255});

    uint32_t x = 0;

    while (!quit)
    {
        linden::utils::ScopedTimer timer;
        linden::utils::FrameRateLimiter limiter(60);

        event_bus.handle_sdl_events();

        level_start_x -= speed;
        if (level_start_x > 100)
        {
            level_start_x = 100;
            speed = 0;
        }

        // Clear screen
        w.get_renderer()->clear({0, 0x44, 0, 0xff});

        // Draw the sky
        SDL_SetRenderDrawColor(w.get_renderer()->get_sdl2_renderer_handle(), 28,
                               150, 197, 0xff);
        SDL_Rect air = {0, 0, 1920, 550};
        SDL_RenderFillRect(w.get_renderer()->get_sdl2_renderer_handle(), &air);

        // Draw the road
        SDL_SetRenderDrawColor(w.get_renderer()->get_sdl2_renderer_handle(),
                               169, 168, 184, 0xff);
        SDL_Rect rect = {0, 770, 1920, 190};
        SDL_RenderFillRect(w.get_renderer()->get_sdl2_renderer_handle(), &rect);

        // Draw the lines outside of the road
        SDL_SetRenderDrawColor(w.get_renderer()->get_sdl2_renderer_handle(),
                               240, 245, 255, 0xff);
        for (uint32_t y = 0; y < 2; y++)
            SDL_RenderDrawLine(w.get_renderer()->get_sdl2_renderer_handle(), 0,
                               548 + y, 1920, 548 + y);
        for (uint32_t y = 0; y < 10; y++)
            SDL_RenderDrawLine(w.get_renderer()->get_sdl2_renderer_handle(), 0,
                               760 + y, 1920, 760 + y);
        for (uint32_t y = 0; y < 10; y++)
            SDL_RenderDrawLine(w.get_renderer()->get_sdl2_renderer_handle(), 0,
                               960 + y, 1920, 960 + y);
        for (uint32_t y = 0; y < 10; y++)
            SDL_RenderDrawLine(w.get_renderer()->get_sdl2_renderer_handle(), 0,
                               855 + y, 1920, 855 + y);

        for (uint32_t x = 0; x < 10; x++)
        {
            SDL_RenderDrawLine(w.get_renderer()->get_sdl2_renderer_handle(),
                               (game_assets.get("car")->get_size().width * 8) +
                                   60 + x + level_start_x,
                               760,
                               (game_assets.get("car")->get_size().width * 8) +
                                   -20 + x + level_start_x,
                               770 + 190);

            SDL_RenderDrawLine(w.get_renderer()->get_sdl2_renderer_handle(),
                               (game_assets.get("car")->get_size().width * 8) +
                                   80 + x + level_start_x,
                               760,
                               (game_assets.get("car")->get_size().width * 8) +
                                   00 + x + level_start_x,
                               770 + 190);
        }

        // Render the foliage
        for (int32_t index = 0; index < 100; index++)
            w.get_renderer()->render_texture(
                ltc.get_background_tile(index),
                {.position = {level_start_x + (index * 128), 440}});

        // Render the car
        car_options.position.y = 830 - (95 * lane);
        w.get_renderer()->render_texture(*game_assets.get("car"), car_options);

        // Render the text
        text_texture.set_text(std::to_string(speed) + " km/h");
        w.get_renderer()->render_texture(
            text_texture,
            {.position = {1920 - text_texture.get_size().width - 32,
                          1080 - text_texture.get_size().height - 32}});

        w.get_renderer()->clear();
        // tg.get_tile(0, 0).add_texture(
        //     *game_assets.get("car"), {0, 0},
        //     game_assets.get("car")->get_size(), {0, 0},
        //     game_assets.get("car")->get_size() * 10);
        tg.add_texture_to_tile(*game_assets.get("car"), {0, 0},
                               game_assets.get("car")->get_size() * x++);

        w.get_renderer()->render_texture(tg.get_tile(0, 0), {.position{0, 0}});
        // w.get_renderer()->render_texture(tg.get_tile(1, 0),
        //                                  {.position{128, 0}});
        // w.get_renderer()->render_texture(tg.get_tile(2, 0),
        //                                  {.position{128, 0}});

        // Render the texture
        w.get_renderer()->render();
    }

    return 0;
}
#endif