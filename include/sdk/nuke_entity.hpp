// floason (C) 2026
// Licensed under the MIT License.

// Interface class for a Nuke entity.

#pragma once

namespace nuke
{

class IPhysicsDescriptor;
class ITextureDescriptor;

class IEntity
{
public:
    virtual ~IEntity() = default;

public:
    // Get the entity's physics descriptor.
    virtual IPhysicsDescriptor* GetPhysicsDescriptor() = 0;

    // Get the entity's texture descriptor.
    virtual ITextureDescriptor* GetTextureDescriptor() = 0;
};

}   // namespace nuke