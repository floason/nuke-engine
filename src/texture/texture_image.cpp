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
    type_ = TextureType::IMAGE;

    SDL_Texture* texture = IMG_LoadTexture(engine.renderer, path);
    if (texture == NULL)
        texture = engine.GetMissingTexture();

    texture_.reset(texture);
    path_ = path;
}

// Get the path of the loaded texture if applicable.
const char* TextureImage::GetLoadedPath() const
{
    return path_.c_str();
}

}   // namespace nuke