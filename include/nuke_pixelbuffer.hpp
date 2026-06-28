// floason (C) 2026
// Licensed under the MIT License.

// This header defines a pixel buffer interface which is populated by the engine,
// alongside its parent pixel buffer descriptor class which is created by the
// consumer

#pragma once

#include "nuke_math.hpp"
#include "nuke_color.hpp"

namespace nuke
{

class IPixelBuffer
{
public:
    virtual ~IPixelBuffer() = default;

public:
    // Lock the buffer in place for modification. This returns a pointer
    // to the pixel buffer, casted to nuke::Color. To get the array to
    // each pixel row, call nuke::Color::GetRow().
    virtual Color* Lock() = 0;

    // Get the pitch of this buffer.
    virtual int Pitch() = 0;
    
    // Signal to the engine that the pixel buffer should be copied into
    // video memory.
    virtual void Unlock() = 0;

    // Is the pixel buffer ready to update? This should be called only by
    // engine code.
    virtual bool Ready() = 0;
};

class PixelBufferDescriptor
{
public:
    // Create a new pixel buffer descriptor.
    PixelBufferDescriptor(unsigned width, unsigned height) :
        width(math::max(width, 1U)),
        height(math::max(height, 1U))
    {
    }

public:
    // Get the width of the pixel buffer in pixels.
    unsigned GetWidth()
    {
        return width;
    }

    // Get the height of the pixel buffer in pixels.
    unsigned GetHeight()
    {
        return height;
    }

public:
    IPixelBuffer* buffer = nullptr;

private:
    unsigned width;
    unsigned height;
};

}   // namespace nuke