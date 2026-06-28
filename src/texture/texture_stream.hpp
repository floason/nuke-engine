// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that can be edited directly.

#pragma once

#include "nuke.hpp"
#include "texture_sdl.hpp"

namespace nuke
{

class Engine;

class TextureStream : public TextureSDL
{   
    friend Engine;

public:
    // Create a new streamed texture using a pixel buffer struct.
    TextureStream(PixelBufferDescriptor* descriptor);

    // Destroy the descriptor's pixel buffer instance on de-allocation.
    virtual ~TextureStream();

private:
    PixelBufferDescriptor* descriptor_;
};

}   // namespace nuke