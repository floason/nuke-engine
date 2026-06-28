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
    type_ = TextureType::RECT;
}

// Draw this texture.
// - origin - the top-left origin of the texture on the window
// - size - the output size of the texture on the window
// - crop_offset - top-left point within the texture to start drawing from
// - scale - determines whether the texture should scale to fill the size vector
void TextureRect::Draw(Vector2 origin, 
                       Vector2 size,
                       ITextureDescriptor* descriptor)
{
    Color color = { 255, 255, 255, 255 };
    if (descriptor != nullptr)
        color = descriptor->GetColor();

    SDL_FRect rect = { origin.x, -origin.y, size.x, size.y };
    SDL_SetRenderDrawColor(engine.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(engine.renderer, &rect);
}

}   // namespace nuke