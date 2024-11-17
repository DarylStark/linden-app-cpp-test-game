#pragma once

#include <linden_graphics/models.h>
#include <linden_graphics/sdl2_renderer.h>
#include <linden_graphics/sdl2_target_texture.h>

#include <vector>

namespace linden::test_game
{
    class TileGrid
    {
    private:
        uint32_t _max_rows;
        uint32_t _max_cols;
        linden::graphics::SDL2Renderer& _renderer;
        std::vector<linden::graphics::SDL2TargetTexture> _tiles;

    public:
        TileGrid(linden::graphics::SDL2Renderer& renderer,
                 const linden::graphics::Size grid_size,
                 const linden::graphics::Size tile_size);

        linden::graphics::SDL2TargetTexture& get_tile(uint32_t row,
                                                      uint32_t col);

        void add_texture_to_tile(linden::graphics::SDL2Texture& texture,
                                 linden::graphics::Position position,
                                 linden::graphics::Size target_size);
    };

}  // namespace linden::test_game