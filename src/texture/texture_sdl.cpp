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
void TextureSDL::Draw(Vector2 origin, RenderContext& context)
{
    SDL_FRect source = { 
        context.crop_offset.x, 
        -context.crop_offset.y, 
        ((context.render_size.x > 0 && !context.scale) ? context.render_size.x : texture_->w), 
        ((context.render_size.y > 0 && !context.scale) ? context.render_size.y : texture_->h) 
    };
    SDL_FRect dest = { 
        origin.x, 
        -origin.y, 
        context.render_size.x,
        context.render_size.y
    };
    SDL_RenderTexture(engine.renderer.sdl_renderer, Get(), &source, &dest);
}

// Retrieve the SDL texture instance as a raw pointer.
SDL_Texture* TextureSDL::Get()
{
    return texture_.get();
}

}   // namespace nuke