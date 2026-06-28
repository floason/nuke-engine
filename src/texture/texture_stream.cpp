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
    type_ = TextureType::STREAM;

    SDL_Texture* texture = SDL_CreateTexture(
        engine.renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        descriptor->GetWidth(),
        descriptor->GetHeight()
    );
    if (texture == nullptr)
        return;

    descriptor->buffer = new PixelBuffer(texture);
    texture_.reset(texture);
}

// Destroy the descriptor's pixel buffer instance on de-allocation.
TextureStream::~TextureStream()
{
    delete descriptor_->buffer;
    descriptor_->buffer = nullptr;
}

}   // namespace nuke