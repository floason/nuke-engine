// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that renders text.

#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include "nuke.hpp"
#include "texture_sdl.hpp"

namespace nuke
{

class TextInterface;

class TextureText : public TextureSDL
{
    friend TextInterface;

public:
    // Create a text texture using a text descriptor.
    TextureText(TextDescriptor* descriptor);

    // Delete the text descriptor's text interface instance.
    virtual ~TextureText() override;

// ITexture
public:
    // Get the size of the texture as a vector.
    virtual Vector2 GetSize() override;

public:
    // Draw this texture.
    // - origin - the top-left origin of the texture on the window
    // - size - the output size of the texture on the window
    // - crop_offset - top-left point within the texture to start drawing from
    // - scale - determines whether the texture should scale to fill the size vector
    void Draw(Vector2 origin, RenderContext& context);

private:
    // Initialize the font.
    void initialize();

private:
    using unique_font = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;
    using unique_text = std::unique_ptr<TTF_Text, decltype(&TTF_DestroyText)>;
    unique_font font_;
    unique_text text_;

    TextDescriptor* descriptor_;
};

}   // namespace nuke