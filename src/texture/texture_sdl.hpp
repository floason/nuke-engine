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
    TextureSDL();

public:
    // Get the size of the texture as a vector.
    virtual Vector2 GetSize() override;

    // Draw this texture.
    // - origin - the top-left origin of the texture on the window
    // - size - the output size of the texture on the window
    // - crop_offset - top-left point within the texture to start drawing from
    // - scale - determines whether the texture should scale to fill the size vector
    virtual void Draw(Vector2 origin, 
                      Vector2 size = { 0, 0 }, 
                      ITextureDescriptor* descriptor = nullptr) override;

public:
    // Retrieve the SDL texture instance as a raw pointer.
    SDL_Texture* Get();

protected:
    using unique_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
    unique_texture texture_;
};

}   // namespace nuke