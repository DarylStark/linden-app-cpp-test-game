#pragma once

#include <linden_graphics/models.h>
#include <linden_graphics/sdl2_font.h>
#include <linden_graphics/sdl2_texture.h>

#include <map>
#include <memory>

namespace linden::test_game
{
    class TextureManager
    {
    private:
        linden::graphics::SDL2Renderer& _renderer;
        std::map<std::string, std::shared_ptr<linden::graphics::SDL2Texture>>
            _textures;
        std::map<std::string, std::shared_ptr<linden::graphics::SDL2Font>>
            _fonts;

    public:
        TextureManager(linden::graphics::SDL2Renderer& renderer);

        // Adding textures
        void add(const std::string& name,
                 std::shared_ptr<linden::graphics::SDL2Texture> texture);
        void add_texture_from_file(const std::string& name,
                                   const std::string& path);
        void add_texture_from_spritesheet(
            const std::string& texture_spritesheet_name,
            const std::string& name, const linden::graphics::Position position,
            const linden::graphics::Size size);

        // Retrieving textures
        std::shared_ptr<linden::graphics::SDL2Texture> get(
            const std::string& name) const;
    };
}  // namespace linden::test_game