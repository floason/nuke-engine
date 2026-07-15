// floason (C) 2026
// Licensed under the MIT License.

// This header defines a pixel buffer abstract class which is populated by the 
// engine,alongside its parent pixel buffer descriptor class which is created 
// by the consumer

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

public:
    int Pitch()
    {
        return pitch_;
    }
    
    // Signal to the engine that the pixel buffer should be copied into
    // video memory.
    void Unlock()
    {
        update_ = true;
    }

    // Is the pixel buffer ready to update? This should be called only by
    // engine code.
    bool Ready()
    {
        if (!update_)
            return false;

        update_ = false;
        buffer_ = nullptr;
        pitch_ = 0;
        return true;
    }

protected:
    Color* buffer_  = nullptr;
    int pitch_      = 0;
    bool update_    = false;
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