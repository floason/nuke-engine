// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that can load images from storage.

#include <SDL3_image/SDL_image.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "texture_image.hpp"

namespace nuke
{

REGISTER_TEXTURE("texture_image", const char*, TextureImage);

// Load a texture from an existing image.
TextureImage::TextureImage(const char* path)
{
    type_ = TextureType::TextureImage;

    SDL_Texture* texture = IMG_LoadTexture(engine.renderer.sdl_renderer, path);
    if (texture == nullptr)
    {
        texture = engine.renderer.GetMissingTexture();
        path_ = std::string("MISSING-") + path;
        loaded_ = false;
    }
    else
        path_ = path;

    texture_.reset(texture);
    path_ = path;
}

// Get the path of the loaded texture if applicable.
const char* TextureImage::GetLoadedPath() const
{
    return path_.c_str();
}

// Draw this texture.
// - origin - the top-left origin of the texture on the window
// - size - the output size of the texture on the window
// - crop_offset - top-left point within the texture to start drawing from
// - scale - determines whether the texture should scale to fill the size vector
void TextureImage::Draw(Vector2 origin, RenderContext& context)
{
    static_cast<TextureSDL*>(this)->Draw(origin, context);
}

}   // namespace nuke