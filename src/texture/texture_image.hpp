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

public:
    // Get the path of the loaded texture, if applicable. This is only valid
    // so long as the texture is alive.
    virtual const char* GetLoadedPath() const override;

private:
    std::string path_;
};

}   // namespace nuke