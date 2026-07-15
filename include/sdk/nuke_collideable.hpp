// floason (C) 2026
// Licensed under the MIT License.

// Interface class for a collideable object. Entities can inherit from this 
// in order to define their origin in the physics engine's world space and 
// their respective bounding boxes.

// Collideables must additionaly propagate a PhysicsContext struct to the
// physics engine, which describes the entity's transform and state.s

#pragma once

#include "nuke_math.hpp"

namespace nuke
{

struct PhysicsContext
{
    Vector2 origin;
    Vector2 mins;
    Vector2 maxs;
    float rotation;
};

class ICollideable
{
public:
    // Get a reference to the collideable's physics context struct.
    virtual PhysicsContext& GetPhysicsContext() = 0;
};

}   // namespace nuke