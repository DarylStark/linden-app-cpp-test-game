#include "texture_manager.h"

#include <linden_graphics/sdl2_image_texture.h>
#include <linden_graphics/sdl2_target_texture.h>

namespace linden::test_game
{
    TextureManager::TextureManager(linden::graphics::SDL2Renderer& renderer)
        : _renderer(renderer)
    {
    }

    void TextureManager::add(
        const std::string& name,
        std::shared_ptr<linden::graphics::SDL2Texture> texture)
    {
        _textures[name] = texture;
    }

    void TextureManager::add_texture_from_file(const std::string& name,
                                               const std::string& path)
    {
        _textures[name] = std::make_shared<linden::graphics::SDL2ImageTexture>(
            _renderer, path);
    }

    void TextureManager::add_texture_from_spritesheet(
        const std::string& name, const std::string& texture_spritesheet_name,
        const linden::graphics::Position position,
        const linden::graphics::Size size)
    {
        _textures[name] = std::make_shared<linden::graphics::SDL2TargetTexture>(
            _renderer, *get(texture_spritesheet_name), position, size);
    }

    std::shared_ptr<linden::graphics::SDL2Texture> TextureManager::get(
        const std::string& name) const
    {
        // TODO: Exception when a texture is not found
        return _textures.at(name);
    }
}  // namespace linden::test_game