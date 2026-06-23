// floason (C) 2026
// Licensed under the MIT License.

// A basic color struct which is de-coupled from SDL.

#pragma once

#include <cstdint>

namespace nuke
{
    
// Compatible with SDL_PIXELFORMAT_RGBA32.
struct Color
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

}   // namespace nuke