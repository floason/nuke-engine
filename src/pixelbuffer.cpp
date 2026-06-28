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

// Get the pitch of this buffer.
int PixelBuffer::Pitch()
{
    return pitch_;
}

// Signal to the engine that the pixel buffer should be copied into
// video memory.
void PixelBuffer::Unlock()
{
    update_ = true;
}

// Is the pixel buffer ready to update? This should be called only by
// engine code.
bool PixelBuffer::Ready()
{
    if (!update_)
        return false;
    update_ = false;
    buffer_ = nullptr;
    pitch_ = 0;
    return true;
}

}   // namespace nuke