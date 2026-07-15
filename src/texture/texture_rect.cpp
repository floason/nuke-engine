// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that draws a simple rectangle.

#include "nuke.hpp"
#include "engine.hpp"
#include "texture_rect.hpp"

namespace nuke
{

REGISTER_TEXTURE("texture_rect", TextureRect);

// Set the texture type to TextureType::RECT.
TextureRect::TextureRect()
{
    type_ = TextureType::TextureRect;
}

// Draw this texture.
// - origin - the top-left origin of the texture on the window
// - size - the output size of the texture on the window
// - crop_offset - top-left point within the texture to start drawing from
// - scale - determines whether the texture should scale to fill the size vector
void TextureRect::Draw(Vector2 origin, RenderContext& context)
{
    SDL_FRect rect = { origin.x, -origin.y, context.render_size.x, context.render_size.y };
    SDL_SetRenderDrawColor(engine.renderer.sdl_renderer, 
                           context.color.r, 
                           context.color.g, 
                           context.color.b, 
                           context.color.a);
    SDL_RenderFillRect(engine.renderer.sdl_renderer, &rect);
}

}   // namespace nuke