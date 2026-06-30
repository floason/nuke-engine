// floason (C) 2026
// Licensed under the MIT License.

// Defines the base texture class for all Nuke textures to inherit from.

#include "nuke.hpp"
#include "texture_base.hpp"

namespace nuke
{

// Has the texture loaded successfully?
const bool TextureBase::Loaded() const
{
    return loaded_;
}

// Get the path of the loaded texture, if applicable. This assumes the
// buffer used for the path name has not been changed/de-allocated().
const char* TextureBase::GetLoadedPath() const
{
    return nullptr;
}

// Get the size of the texture as a vector, if applicable.
Vector2 TextureBase::GetSize()
{
    return Vector2(0, 0);
}

}   // namespace nuke