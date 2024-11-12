#include <SDL2/SDL.h>
#include <linden_graphics/sdl2.h>
#include <linden_graphics/sdl2_eventbus.h>
#include <linden_graphics/sdl2_image_texture.h>
#include <linden_graphics/sdl2_target_texture.h>
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
    linden::graphics::SDL2ImageTexture car_texture(*w.get_renderer(),
                                                   "assets/car.png");
    std::map<std::string, std::shared_ptr<linden::graphics::SDL2Texture>> trees;
    trees["tree_04"] = std::make_shared<linden::graphics::SDL2TargetTexture>(
        *w.get_renderer(), foliage_texture,
        linden::graphics::Position({478, 0}),
        linden::graphics::Size({102, 287}));
    trees["tree_10"] = std::make_shared<linden::graphics::SDL2TargetTexture>(
        *w.get_renderer(), foliage_texture,
        linden::graphics::Position({0, 403}),
        linden::graphics::Size({151, 211}));

    // Create the scene
    linden::graphics::SDL2TargetTexture scene(*w.get_renderer(), {5000, 300});
    scene.add_texture(*trees["tree_04"], {0, 0}, trees["tree_04"]->get_size(),
                      {100, 20}, trees["tree_04"]->get_size());
    scene.add_texture(*trees["tree_04"], {0, 0}, trees["tree_04"]->get_size(),
                      {252, 0}, trees["tree_04"]->get_size());
    scene.add_texture(*trees["tree_04"], {0, 0}, trees["tree_04"]->get_size(),
                      {402, 40}, trees["tree_04"]->get_size());
    scene.add_texture(*trees["tree_10"], {0, 0}, trees["tree_10"]->get_size(),
                      {542, 60}, trees["tree_10"]->get_size());

    scene.add_texture(*trees["tree_10"], {0, 0}, trees["tree_10"]->get_size(),
                      {1942, 60}, trees["tree_10"]->get_size());
    scene.add_texture(*trees["tree_04"], {0, 0}, trees["tree_04"]->get_size(),
                      {2542, 40}, trees["tree_04"]->get_size());

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    int32_t level_start_x = 100;

    linden::graphics::Position rotate_center_accelerate = {
        0, car_texture.get_size().height - 1};
    linden::graphics::Position rotate_center_deaccelerate = {
        car_texture.get_size().width - 1, car_texture.get_size().height - 1};

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

    while (!quit)
    {
        // linden::utils::ScopedTimer timer;
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
                               0x55, 0x55, 0x55, 0xff);
        SDL_Rect rect = {0, 770, 1920, 190};
        SDL_RenderFillRect(w.get_renderer()->get_sdl2_renderer_handle(), &rect);

        // Draw the lines outside of the road
        SDL_SetRenderDrawColor(w.get_renderer()->get_sdl2_renderer_handle(),
                               0xaa, 0xaa, 0xaa, 0xff);
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
            SDL_RenderDrawLine(
                w.get_renderer()->get_sdl2_renderer_handle(),
                (car_texture.get_size().width * 8) + 60 + x + level_start_x,
                760,
                (car_texture.get_size().width * 8) + -20 + x + level_start_x,
                770 + 190);

            SDL_RenderDrawLine(
                w.get_renderer()->get_sdl2_renderer_handle(),
                (car_texture.get_size().width * 8) + 80 + x + level_start_x,
                760,
                (car_texture.get_size().width * 8) + 00 + x + level_start_x,
                770 + 190);
        }

        // Render the foliage
        w.get_renderer()->render_texture(scene,
                                         {.position = {level_start_x, 440}});

        // Render the car
        car_options.position.y = 830 - (95 * lane);
        w.get_renderer()->render_texture(car_texture, car_options);

        // Render the texture
        w.get_renderer()->render();
    }

    return 0;
}