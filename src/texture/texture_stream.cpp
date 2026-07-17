// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that can be edited directly.

#include "nuke.hpp"
#include "engine.hpp"
#include "texture_stream.hpp"
#include "pixelbuffer.hpp"

namespace nuke
{

REGISTER_TEXTURE("texture_stream", PixelBufferDescriptor*, TextureStream);

// Create a new streamed texture using a pixel buffer struct.
TextureStream::TextureStream(PixelBufferDescriptor* descriptor) :
    descriptor_(descriptor)
{
    type_ = TextureType::TextureStream;

    if (descriptor != nullptr)
    {
        texture_ = SDL_CreateTexture(
            engine.renderer.sdl_renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            descriptor->GetWidth(),
            descriptor->GetHeight()
        );
    }
    if (texture_ == nullptr)
    {
        texture_ = engine.renderer.GetMissingTexture();
        loaded_ = false;
    }

    descriptor->buffer = new PixelBuffer(texture_);
}

// Destroy the descriptor's pixel buffer instance on de-allocation.
TextureStream::~TextureStream()
{
    delete descriptor_->buffer;
    descriptor_->buffer = nullptr;
}

// Draw this texture.
// - origin - the top-left origin of the texture on the window
// - size - the output size of the texture on the window
// - crop_offset - top-left point within the texture to start drawing from
// - scale - determines whether the texture should scale to fill the size vector
void TextureStream::Draw(Vector2 origin, RenderContext& context)
{
    // Unlock the texture if the pixel buffer is ready to update. This is
    // required fro copying the pixel buffer contents back into video memory.
    if (descriptor_->buffer != nullptr && descriptor_->buffer->Ready())
        SDL_UnlockTexture(texture_);

    static_cast<TextureSDL*>(this)->Draw(origin, context);
}

}   // namespace nuke