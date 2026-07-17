// floason (C) 2026
// Licensed under the MIT License.

// Defines a texture that renders text.

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "texture_text.hpp"
#include "text_interface.hpp"

namespace nuke
{

REGISTER_TEXTURE("texture_text", TextDescriptor*, TextureText);

// Create a text texture using a text descriptor.
TextureText::TextureText(TextDescriptor* descriptor) :
    font_(nullptr, TTF_CloseFont),
    text_(nullptr, TTF_DestroyText),
    descriptor_(descriptor)
{
    type_ = TextureType::TextureText;

    // Attempt to initialize the font prior to rendering.
    initialize();

    // If the text instance failed to load at the end, the "missing texture"
    // texture must be used in place instead.
    texture_ = engine.renderer.GetMissingTexture();
}

// Delete the text descriptor's text interface instance.
TextureText::~TextureText()
{
    if (descriptor_ != nullptr && descriptor_->interface_ != nullptr)
        delete descriptor_->interface_;
}

// Get the size of the texture as a vector.
Vector2 TextureText::GetSize()
{
    TTF_Text* text = text_.get();
    if (text == nullptr)
        return Vector2(0, 0);
        
    int x;
    int y;
    TTF_GetTextSize(text, &x, &y);
    return Vector2(x, y);
}

// Draw this texture.
// - origin - the top-left origin of the texture on the window
// - size - the output size of the texture on the window
// - crop_offset - top-left point within the texture to start drawing from
// - scale - determines whether the texture should scale to fill the size vector
void TextureText::Draw(Vector2 origin, RenderContext& context)
{
    TTF_Text* text = text_.get();
    if (text != nullptr)
        TTF_DrawRendererText(text, origin.x, -origin.y);
    else
    {
        // Only default to TextureSDL's drawing mechanism to draw the invalid
        // texture if the font failed to load.
        static_cast<TextureSDL*>(this)->Draw(origin, context);
    }
}

// Initialize the font.
void TextureText::initialize()
{
    TTF_Text* text = nullptr;
    std::string buffer = descriptor_->GetBuffer();
    if (descriptor_ == nullptr)
        return;

    if (descriptor_->interface_ == nullptr)
        descriptor_->interface_ = new TextInterface(this);

    TTF_Font* font = TTF_OpenFont(descriptor_->GetFont().c_str(), descriptor_->GetFontSize());
    bool update_text = true;
    if (font != nullptr)
    {
        text = text_.get();
        if (text_.get() == nullptr)
        {
            text = TTF_CreateText(
                engine.renderer.sdl_text_engine,
                font,
                buffer.c_str(),
                buffer.length()
            );
        }
        else
        {
            // If only the font was updated, just update the text's font
            // attribute.
            TTF_SetTextFont(text, font);
            update_text = false;
        }
    }

    // Reset the font/text instance pointers. The text pointer is only reset if
    // a new text instance was created.
    font_.reset(font);
    if (update_text)
        text_.reset(text);

    // If we got to the point that the text instance was created successfully,
    // tell the text descriptor that the font was initialized.
    if (text != nullptr)
    {
        descriptor_->SetFontInitialized();

        // Ensure that font-specific attributes are updated.
        descriptor_->interface_->Update(ITextInterface::ATTRIBUTE_FONT_STYLE);
        descriptor_->interface_->Update(ITextInterface::ATTRIBUTE_HORIZONTAL_ALIGNMENT);
    }
}

}   // namespace nuke