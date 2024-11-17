#include "background_generator.h"

namespace linden::test_game
{
    BackgroundGenerator::BackgroundGenerator(
        const std::shared_ptr<linden::graphics::SDL2Renderer>& renderer,
        const linden::test_game::TextureManager& texture_manager,
        const LevelData& level_data)
        : _renderer(renderer), _texture_manager(texture_manager),
          _level_data(level_data)
    {
    }

    linden::graphics::Size BackgroundGenerator::_get_tile_size() const
    {
        // TODO: height should be calculated based on the screen size
        return linden::graphics::Size{TILE_WIDTH, TILE_HEIGHT};
    }

    void BackgroundGenerator::_create_background_textures()
    {
        // Calculate how many textures are needed
        uint32_t needed_width = _level_data.length_in_meters * PIXELS_PER_METER;
        uint32_t number_of_textures = (needed_width / TILE_WIDTH);
        uint32_t last_texture_width = needed_width % TILE_WIDTH;
        if (last_texture_width > 0) number_of_textures++;

        // Configure the vector to hold the textures
        _background_textures.reserve(number_of_textures + 1);

        // Create empty textures
        for (uint32_t i = 0; i < number_of_textures; i++)
            _background_textures.emplace_back(*_renderer, _get_tile_size());

        // Add the last texture
        if (last_texture_width > 0)
            _background_textures.emplace_back(
                *_renderer,
                linden::graphics::Size{static_cast<int>(last_texture_width),
                                       TILE_HEIGHT});

        // TODO: add them in the correct texture
        add_texture_to_background("tree_04", {0, 20});
        add_texture_to_background("tree_04", {100, 20});
        add_texture_to_background("tree_04", {252, 0});
        add_texture_to_background("tree_04", {402, 40});
        add_texture_to_background("tree_10", {542, 60});
        add_texture_to_background("tree_10", {1942, 60});
        add_texture_to_background("tree_04", {2542, 40});
    }

    void BackgroundGenerator::add_texture_to_background(
        const std::string& texture_name, linden::graphics::Position position)
    {
        // Get the correct texture
        linden::graphics::SDL2Texture& texture =
            *_texture_manager.get(texture_name);

        // Find the tile where the texture should be added
        const int tile_index = position.x / TILE_WIDTH;
        if (tile_index >= _background_textures.size())
            throw std::runtime_error(
                "Texture index out of bounds");  // TODO: use a custom exception

        // Add the texture
        add_texture_to_tile(tile_index, texture, position);

        // Check if we need to add it to more than one tile
        if (position.x + texture.get_size().width > TILE_WIDTH)
        {
            add_texture_to_tile(
                tile_index + 1, texture,
                {position.x - static_cast<int32_t>(TILE_WIDTH), position.y});
        }
    }

    void BackgroundGenerator::add_texture_to_tile(
        const int tile_index, linden::graphics::SDL2Texture& texture,
        linden::graphics::Position position)
    {
        // Add the texture
        _background_textures[tile_index].add_texture(
            texture, {0, 0}, texture.get_size(), position, texture.get_size());
    }

    const linden::graphics::SDL2TargetTexture&
    BackgroundGenerator::get_background_tile(uint32_t tile_index)
    {
        // TODO: Check for valid tile index
        // TODO: Make sure the user can request specific background position,
        //       width and height and retrieve the needed tiles.

        if (_background_textures.size() == 0) _create_background_textures();
        return _background_textures[tile_index];
    }
}  // namespace linden::test_game