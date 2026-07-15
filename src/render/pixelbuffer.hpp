// floason (C) 2026
// Licensed under the MIT License.

// This defines a pixel buffer class which incorporates the IPixelBuffer
// interface.

#pragma once

#include <SDL3/SDL.h>

#include "nuke.hpp"

namespace nuke
{

class PixelBuffer : public IPixelBuffer
{
public:
    // Aggregate the relevant SDL texture instance.
    PixelBuffer(SDL_Texture* texture_reference);

// IPixelBuffer
public:
    // Lock the buffer in place for modification. This returns a pointer
    // to the pixel buffer, casted to nuke::Color. To get the array to
    // each pixel row, call nuke::Color::GetRow().
    virtual Color* Lock() override;

private:
    SDL_Texture* texture_reference_;
};

};  // namespace nuke