// floason (C) 2026
// Licensed under the MIT License.

// Alows interactivity between a text descriptor instance and its
// respective text texture so as to update the texture's attributes.

#include "nuke.hpp"
#include "texture.hpp"
#include "text_interface.hpp"

namespace nuke
{

// Bind the designated text texture instance.
TextInterface::TextInterface(TextureText* texture) :
    texture_(texture)
{
}

// Invoke the text texture to update the desired attribute on its end.
bool TextInterface::Update(Attribute attribute)
{
    if (texture_ == nullptr)
    {
        assert(false && "Texture pointer is nullptr");
        return false;
    }
    
    TTF_Text* text = texture_->text_.get();
    if (text == nullptr && attribute != ITextInterface::ATTRIBUTE_FONT_NAME)
    {
        assert(false && "Text pointer is nullptr");
        return false;
    }

    switch (attribute)
    {
        case ITextInterface::ATTRIBUTE_BUFFER:
        {
            std::string buffer = texture_->descriptor_->GetBuffer();
            TTF_SetTextString(text, buffer.c_str(), buffer.length());
            break;
        };

        case ITextInterface::ATTRIBUTE_FONT_NAME:
        {
            texture_->initialize();
            break;
        }

        case ITextInterface::ATTRIBUTE_FONT_SIZE:
        {
            TTF_SetFontSize(texture_->font_.get(), texture_->descriptor_->GetFontSize());
            break;
        }

        case ITextInterface::ATTRIBUTE_FONT_STYLE:
        {
            TTF_SetFontStyle(
                texture_->font_.get(), 
                static_cast<TTF_FontStyleFlags>(texture_->descriptor_->GetFontStyleFlags())
            );
            break;
        }

        case ITextInterface::ATTRIBUTE_WRAP_WIDTH:
        {
            TTF_SetTextWrapWidth(text, texture_->descriptor_->GetWrapWidth());
            break;
        }

        case ITextInterface::ATTRIBUTE_COLOR:
        {
            Color color = texture_->descriptor_->GetColor();
            TTF_SetTextColor(text, color.r, color.g, color.b, color.a);
            break;
        }

        case ITextInterface::ATTRIBUTE_HORIZONTAL_ALIGNMENT:
        {
            TTF_SetFontWrapAlignment(
                texture_->font_.get(), 
                static_cast<TTF_HorizontalAlignment>(texture_->descriptor_->GetHorizontalWrapAlignment())
            );
            break;
        }

        default:
            assert(false && "Unknown text attribute");
            return false;
    }

    return true;
}
    
}   // namespace nuke