#pragma once
#include <linden_graphics/sdl2_target_texture.h>

#include <memory>
#include <vector>

#include "../resource_manager/texture_manager.h"
#include "level_data.h"

namespace linden::test_game
{
    class BackgroundGenerator
    {
    private:
        constexpr static uint32_t PIXELS_PER_METER = 100;
        constexpr static uint32_t TILE_WIDTH = 1024;
        constexpr static uint32_t TILE_HEIGHT = 300;

        // Dependencies
        const std::shared_ptr<linden::graphics::SDL2Renderer>& _renderer;
        const linden::test_game::TextureManager& _texture_manager;
        const LevelData& _level_data;

        // Textures
        std::vector<linden::graphics::SDL2TargetTexture> _background_textures;

        linden::graphics::Size _get_tile_size() const;
        void _create_background_textures();

    public:
        BackgroundGenerator(
            const std::shared_ptr<linden::graphics::SDL2Renderer>& renderer,
            const linden::test_game::TextureManager& texture_manager,
            const LevelData& level_data);

        // Add textures to the background
        void add_texture_to_background(const std::string& texture_name,
                                       linden::graphics::Position position);
        void add_texture_to_tile(const int tile_index,
                                 linden::graphics::SDL2Texture& texture,
                                 linden::graphics::Position position);

        // Get textures
        const linden::graphics::SDL2TargetTexture& get_background_tile(
            uint32_t tile_index);
    };
}  // namespace linden::test_game