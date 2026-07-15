// floason (C) 2026
// Licensed under the MIT License.

// A struct defining common variables shared between the engine and game.

#pragma once

#include <cstdint>

namespace nuke
{

struct CommonVars
{
    float frametime     = 0.f;  // Delta time between current and last frame.
    float curtime       = 0.f;  // ticks * tick_interval
    float realtime      = 0.f;  // Real time elapsed since engine start.
    float tick_interval = 0.f;  // Constant - delta time between ticks.
    uint64_t frames     = 0;    // No. frames since engine start (starting from 0).
    uint64_t ticks      = 0;    // No. ticks since engine stat (starting from 0).
};

}   // namespace nuke