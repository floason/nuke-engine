// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that can be edited directly.

#pragma once

#include "nuke.hpp"
#include "texture_sdl.hpp"

namespace nuke
{

class Renderer;

class TextureStream : public TextureSDL
{   
    friend Renderer;

public:
    // Create a new streamed texture using a pixel buffer struct.
    TextureStream(PixelBufferDescriptor* descriptor);

    // Destroy the descriptor's pixel buffer instance on de-allocation.
    virtual ~TextureStream();

public:
    // Draw this texture.
    // - origin - the top-left origin of the texture on the window
    // - size - the output size of the texture on the window
    // - crop_offset - top-left point within the texture to start drawing from
    // - scale - determines whether the texture should scale to fill the size vector
    void Draw(Vector2 origin, RenderContext& context);

private:
    PixelBufferDescriptor* descriptor_;
};

}   // namespace nuke