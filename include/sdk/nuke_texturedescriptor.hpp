// floason (C) 2026
// Licensed under the MIT License.

// Interface class for the texture descriptor which describes the texture
// an entity and associated rendering information.

#pragma once

#include "nuke_texture.hpp"
#include "nuke_math.hpp"

namespace nuke
{

class IEntity;

class ITextureDescriptor
{
public:
    // Get the described entity.
    virtual IEntity* GetEntity() = 0;

     // Get the entity's texture.
    virtual ITexture* GetTexture() = 0;

    // Set the entity's texture.
    virtual void SetTexture(ITexture* texture, bool set_bounds = false) = 0;

    // Get a reference to the render size vector of the texture.
    virtual Vector2& GetRenderSize() = 0;

    // Get a reference to the crop offset vector when drawing the texture.
    virtual Vector2& GetCropOffset() = 0;

    // Get a reference to the color struct of the texture.
    virtual Color& GetColor() = 0;

    // Get a reference to the scale property for deciding whether to scale
    // the drawn texture to fit its render size.
    virtual bool& GetScale() = 0;

    // Set ownership of the loaded texture to this entity, if applicable.
    virtual void OwnTexture() = 0;
};

}   // namespace nuke