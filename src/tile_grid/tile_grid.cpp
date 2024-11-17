#include "tile_grid.h"

#include <algorithm>

namespace linden::test_game
{
    TileGrid::TileGrid(linden::graphics::SDL2Renderer& renderer,
                       const linden::graphics::Size grid_size,
                       const linden::graphics::Size tile_size)
        : _renderer(renderer)
    {
        _max_rows =
            (grid_size.height + tile_size.height - 1) / tile_size.height;
        _max_cols = (grid_size.width + tile_size.width - 1) / tile_size.width;

        // Reserve the memory for the grid
        _tiles.reserve(_max_cols * _max_rows);

        // Create empty tiles
        std::generate_n(std::back_inserter(_tiles), _max_cols * _max_rows,
                        [&renderer, &tile_size]() {
                            return linden::graphics::SDL2TargetTexture(
                                renderer, tile_size);
                        });
    }

    linden::graphics::SDL2TargetTexture& TileGrid::get_tile(uint32_t row,
                                                            uint32_t col)
    {
        return _tiles[row * _max_cols + col];
    }

    void TileGrid::add_texture_to_tile(linden::graphics::SDL2Texture& texture,
                                       linden::graphics::Position position,
                                       linden::graphics::Size target_size)
    {
        // TODO: This should be spread accross multiple tiles if needed

        // Find the tile where the texture should be added
        const int tile_index = position.x / _tiles[0].get_size().width;

        // Add the texture to the tile
        _tiles[tile_index].add_texture(texture, position, texture.get_size(),
                                       {0, 0}, target_size);
    }
}  // namespace linden::test_game