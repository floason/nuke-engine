// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that holds an SDL_Texture* instance.

#include "nuke.hpp"
#include "engine.hpp"
#include "texture_sdl.hpp"

namespace nuke
{

TextureSDL::TextureSDL() :
    texture_(nullptr, SDL_DestroyTexture)
{
}

// Get the size of the texture as a vector.
Vector2 TextureSDL::GetSize()
{
    SDL_Texture* texture = Get();
    if (texture == nullptr)
        return Vector2(0, 0);
    return Vector2(texture->w, texture->h);
}

// Draw this texture.
// - origin - the top-left origin of the texture on the window
// - size - the output size of the texture on the window
// - crop_offset - top-left point within the texture to start drawing from
// - scale - determines whether the texture should scale to fill the size vector
void TextureSDL::Draw(Vector2 origin,
                      Vector2 size, 
                      ITextureDescriptor* descriptor)
{
    Vector2 crop_offset = { 0, 0 };
    bool scale = false;
    if (descriptor != nullptr)
    {
        crop_offset = descriptor->GetCropOffset();
        scale = descriptor->GetScale();
    }

    SDL_FRect source = { 
        crop_offset.x, 
        -crop_offset.y, 
        ((size.x > 0 && !scale) ? size.x : texture_->w), 
        ((size.y > 0 && !scale) ? size.y : texture_->h) 
    };
    SDL_FRect dest = { 
        origin.x, 
        -origin.y, 
        size.x,
        size.y
    };
    SDL_RenderTexture(engine.renderer, Get(), &source, &dest);
}

// Retrieve the SDL texture instance as a raw pointer.
SDL_Texture* TextureSDL::Get()
{
    return texture_.get();
}

}   // namespace nuke