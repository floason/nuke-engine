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
    Vector2 origin      = { 0.f, 0.f };
    Vector2 velocity    = { 0.f, 0.f };
    Vector2 mins        = { 0.f, 0.f };
    Vector2 maxs        = { 0.f, 0.f };
    float rotation      = 0.f;
};

class ICollideable
{
public:
    // Get a reference to the collideable's physics context struct.
    virtual PhysicsContext& GetPhysicsContext() = 0;
};

}   // namespace nuke