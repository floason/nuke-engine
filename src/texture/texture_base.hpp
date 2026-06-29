// floason (C) 2026
// Licensed under the MIT License.

// Defines the base texture class for all Nuke textures to inherit from.

#pragma once

#include "nuke.hpp"
#include "texture_factory.hpp"

namespace nuke
{

enum class TextureType
{
    BASE,   // This texture is not usable!

    RECT,
    IMAGE,
    STREAM
};

class TextureBase : public ITexture
{
public:
    // Get the path of the loaded texture, if applicable. This assumes the
    // buffer used for the path name has not been changed/de-allocated().
    virtual const char* GetLoadedPath() const override;

    // Get the size of the texture as a vector, if applicable.
    virtual Vector2 GetSize() override;

public:
    // Draw this texture.
    // - origin - the top-left origin of the texture on the window
    // - size - the output size of the texture on the window
    // - crop_offset - top-left point within the texture to start drawing from
    // - scale - determines whether the texture should scale to fill the size vector
    virtual void Draw(Vector2 origin, 
                      Vector2 size = { 0, 0 }, 
                      ITextureDescriptor* descriptor = nullptr) = 0;

public:
    // Get the texture type.
    TextureType GetType();

protected:
    TextureType type_ = TextureType::BASE;
};

}   // namespace nuke