// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that can load images from storage.

#pragma once

#include <string>

#include "nuke.hpp"
#include "texture_sdl.hpp"

namespace nuke
{

class TextureImage : public TextureSDL
{
public:
    // Load a texture from an existing image.
    TextureImage(const char* path);

// ITexture
public:
    // Get the path of the loaded texture, if applicable. This is only valid
    // so long as the texture is alive.
    virtual const char* GetLoadedPath() const override;

public:
    // Draw this texture.
    // - origin - the top-left origin of the texture on the window
    // - size - the output size of the texture on the window
    // - crop_offset - top-left point within the texture to start drawing from
    // - scale - determines whether the texture should scale to fill the size vector
    void Draw(Vector2 origin, RenderContext& context);

private:
    std::string path_;
};

}   // namespace nuke