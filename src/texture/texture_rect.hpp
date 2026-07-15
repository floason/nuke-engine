// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that draws a simple rectangle.

#pragma once

#include "nuke.hpp"
#include "texture_base.hpp"

namespace nuke
{

class TextureRect : public TextureBase
{
public:
    // Set the texture type to TextureType::RECT.
    TextureRect();

public:
    // Draw this texture.
    // - origin - the top-left origin of the texture on the window
    // - size - the output size of the texture on the window
    // - crop_offset - top-left point within the texture to start drawing from
    // - scale - determines whether the texture should scale to fill the size vector
    void Draw(Vector2 origin, RenderContext& context);
};

}   // namespace nuke