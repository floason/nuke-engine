// floason (C) 2026
// Licensed under the MIT License.

// Interface class for the physics descriptor for entities.

#pragma once

#include "nuke_math.hpp"

namespace nuke
{

class IEntity;

class IPhysicsDescriptor
{
public:
    // Get the described entity.
    virtual IEntity* GetEntity() = 0;

    // Get a reference to the the entity's world origin.
    virtual Vector2& GetOrigin() = 0;

    // Get a reference to the entity's minimum bounding box vector.
    virtual Vector2& GetMins() = 0;

    // Get a reference to the entity's maximum bounding box vector.
    virtual Vector2& GetMaxs() = 0;

    // Get a reference to the entity's rotation attribute.
    virtual float& GetRotation() = 0;
};

}   // namespace nuke