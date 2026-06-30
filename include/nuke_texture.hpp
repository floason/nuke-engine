// floason (C) 2026
// Licensed under the MIT License.

// Interface class for describing entity texture information.

#pragma once

#include "nuke_math.hpp"
#include "nuke_color.hpp"

namespace nuke
{

class ITexture
{
public:
    virtual ~ITexture() = default;

public:
    // Has the texture loaded successfully?
    virtual const bool Loaded() const = 0;

    // Get the path of the loaded texture, if applicable. This is only valid
    // so long as the texture is alive.
    virtual const char* GetLoadedPath() const = 0;

    // Get the size of the texture as a vector, if applicable.
    virtual Vector2 GetSize() = 0;
};

}   // namespace nuke