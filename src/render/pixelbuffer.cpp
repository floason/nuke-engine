// floason (C) 2026
// Licensed under the MIT License.

// This defines a pixel buffer class which incorporates the IPixelBuffer
// interface.

#include "nuke.hpp"
#include "pixelbuffer.hpp"

namespace nuke
{

// Aggregate the relevant SDL texture instance.
PixelBuffer::PixelBuffer(SDL_Texture* texture_reference) :
    texture_reference_(texture_reference)
{
}

// Lock the buffer in place for modification. This returns a pointer
// to the pixel buffer, casted to nuke::Color. To get the array to
// each pixel row, call nuke::Color::GetRow().
Color* PixelBuffer::Lock()
{
    if (!update_ && buffer_ != nullptr)
        return buffer_;

    update_ = false;
    SDL_LockTexture(texture_reference_, NULL, reinterpret_cast<void**>(&buffer_), &pitch_);
    return buffer_;
}

}   // namespace nuke