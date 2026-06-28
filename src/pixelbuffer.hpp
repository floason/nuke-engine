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

public:
    // Lock the buffer in place for modification. This returns a pointer
    // to the pixel buffer, casted to nuke::Color. To get the array to
    // each pixel row, call nuke::Color::GetRow().
    virtual Color* Lock() override;

    // Get the pitch of this buffer.
    virtual int Pitch() override;
    
    // Signal to the engine that the pixel buffer should be copied into
    // video memory.
    virtual void Unlock() override;

    // Is the pixel buffer ready to update? This should be called only by
    // engine code.
    virtual bool Ready() override;

private:
    SDL_Texture* texture_reference_;
    Color* buffer_                      = nullptr;
    int pitch_                          = 0;
    bool update_                        = false;
};

};  // namespace nuke