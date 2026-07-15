// floason (C) 2026
// Licensed under the MIT License.

// A basic structure which defines vectors for the engine camera view.

#pragma once

#include "nuke_math.hpp"

namespace nuke
{

struct CameraContext
{
    // Sets the offset of the camera view. This shifts the origins of all
    // renderables in the inverse direction.
    Vector2 camera_offset       = { 0.f, 0.f };
    
    // Audio attenuation is calculated relative to the offset into the
    // camera view set by this vector.
    Vector2 attenuation_offset  = { 0.f, 0.f };

    // Audio Doppler shifting is calculated relative to the speed of the
    // viewed entity. This vector should be updated to match the velocity 
    // of the viewed player.
    Vector2 doppler_velocity    = { 0.f, 0.f };
};

}   // namespace nuke