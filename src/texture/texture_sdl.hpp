// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that holds an SDL_Texture* instance.

#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "nuke.hpp"
#include "texture_base.hpp"

namespace nuke
{

class TextureSDL : public TextureBase
{
public:
    // Destroy the SDL texture if owned by this texture instance.
    virtual ~TextureSDL() override;

// ITexture
public:
    // Get the size of the texture as a vector.
    virtual Vector2 GetSize() override;

public:
    // Draw this texture.
    // - origin - the top-left origin of the texture on the window
    // - size - the output size of the texture on the window
    // - crop_offset - top-left point within the texture to start drawing from
    // - scale - determines whether the texture should scale to fill the size vector
    void Draw(Vector2 origin, RenderContext& context);

protected:
    SDL_Texture* texture_   = nullptr;
};

}   // namespace nuke