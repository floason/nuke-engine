// floason (C) 2026
// Licensed under the MIT License.

// Defines the base texture class for all Nuke textures to inherit from.

#pragma once

#include "nuke.hpp"
#include "texture_factory.hpp"

#define TEXTURE_TYPE_MACROS \
    X(TextureRect)          \
    X(TextureImage)         \
    X(TextureStream)        \
    X(TextureText)

namespace nuke
{

enum class TextureType
{
    // The following manually specified types cannot be drawn!
    TextureBase,
    TextureTest,

#define X(TYPE) TYPE,
    TEXTURE_TYPE_MACROS
#undef X
};

// Base texture class. This can not be drawn and all texture classes must
// inherit from this class.
class TextureBase : public ITexture
{
// ITexture
public:
    // Has the texture loaded successfully?
    virtual const bool Loaded() const override;

    // Get the path of the loaded texture, if applicable. This assumes the
    // buffer used for the path name has not been changed/de-allocated().
    virtual const char* GetLoadedPath() const override;

    // Get the size of the texture as a vector, if applicable.
    virtual Vector2 GetSize() override;

public:
    // Get the texture type.
    inline TextureType GetType()
    {
        return type_;
    }

protected:
    TextureType type_ = TextureType::TextureBase;
    bool loaded_ = true;
};

}   // namespace nuke