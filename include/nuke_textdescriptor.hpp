// floason (C) 2026
// Licensed under the MIT License.

// Describes the text buffer and font information of a text texture.
// The TextDescriptor class additionally aggregates a text interface
// component for interfacing with the text texture when updating text
// attributes.

#pragma once

#include <string>

#include "nuke_color.hpp"

namespace nuke
{

class TextureText;

class ITextInterface
{
public:
    virtual ~ITextInterface() = default;

public:
    enum Attribute
    {
        ATTRIBUTE_BUFFER,
        ATTRIBUTE_FONT_NAME,
        ATTRIBUTE_FONT_SIZE,
        ATTRIBUTE_FONT_STYLE,
        ATTRIBUTE_WRAP_WIDTH,
        ATTRIBUTE_COLOR,
        ATTRIBUTE_HORIZONTAL_ALIGNMENT
    };

public:
    // Invoke the text texture to update the desired attribute on its
    // end.
    virtual bool Update(Attribute attribute) = 0;
};

class TextDescriptor
{
    friend TextureText;

public:
    enum HorizontalAlignment
    {
        HORIZONTAL_LEFT,
        HORIZONTAL_CENTRE,
        HOIRZONTAL_RIGHT
    };

    enum FontStyleFlags
    {
        FONT_STYLE_NONE             = 0,

        FONT_STYLE_BOLD             = (1 << 0),
        FONT_STYLE_ITALIC           = (1 << 1),
        FONT_STYLE_UNDERLINE        = (1 << 2),
        FONT_STYLE_STRIKETHROUGH    = (1 << 3)
    };

public:
    // Set the font used. Any additional text attributes cannot be set 
    // until a font has been successfully loaded. If the font is left
    // blank, the engine will load a default font instead.
    TextDescriptor(std::string font = "", float font_size = 12.f) :
        font_name_(font),
        font_size_(font_size)
    {
        // The texture attempts to instantiate its font instance after
        // the text descriptor has been bound, thus the update method
        // does not need to be invoked here.
    }

public:
    // Get the string buffer.
    inline std::string GetBuffer() 
    {
        return buffer_;
    }

    // Update the string buffer.
    inline void SetBuffer(std::string buffer)
    {
        buffer_ = buffer;
        updateText(ITextInterface::ATTRIBUTE_BUFFER);
    }

    // Get the font style.
    inline std::string GetFont()
    {
        return font_name_;
    }

    // Set the font style. If left blank, the engine default font
    // will be loaded instead.
    inline void SetFont(std::string font = "")
    {
        font_initialized_ = false;
        font_name_ = font;
        updateText(ITextInterface::ATTRIBUTE_FONT_NAME);
    }
    
    // Get the font size.
    inline float GetFontSize()
    {
        return font_size_;
    }

    // Set the font size.
    inline void SetFontSize(float font_size)
    {
        font_size_ = font_size;
        updateText(ITextInterface::ATTRIBUTE_FONT_SIZE);
    }

    // Get the font style flags.
    inline FontStyleFlags GetFontStyleFlags()
    {
        return font_style_flags_;
    }

    // Set the font style flags.
    inline void SetFontStyleFlags(FontStyleFlags flags)
    {
        font_style_flags_ = flags;
        updateText(ITextInterface::ATTRIBUTE_FONT_STYLE);
    }

    // Get the wrap width.
    inline int GetWrapWidth()
    {
        return wrap_width_;
    }

    // Set the wrap width in pixels. Set to zero to wrap only on new lines.
    inline void SetWrapWidth(int wrap_width)
    {
        wrap_width_ = wrap_width;
        updateText(ITextInterface::ATTRIBUTE_WRAP_WIDTH);
    }

    // Get the text color.
    inline Color GetColor()
    {
        return color_;
    }

    // Set the text color.
    inline void SetColor(Color color)
    {
        color_ = color;
        updateText(ITextInterface::ATTRIBUTE_COLOR);
    }

    // Get the horizontal wrap alignment.
    inline HorizontalAlignment GetHorizontalWrapAlignment()
    {
        return horizontal_alignment_;
    }

    // Set the horizontal wrap alignment.
    inline void SetHorizontalWrapAlignment(HorizontalAlignment alignment)
    {
        horizontal_alignment_ = alignment;
        updateText(ITextInterface::ATTRIBUTE_HORIZONTAL_ALIGNMENT);
    }

    // Has the font been initialized?
    inline bool IsFontInitialized()
    {
        return font_initialized_;
    }

    // Has font (re-)initialization been reqeusted?
    inline bool FontReinitializeRequest()
    {
        return request_font_initialize_ && !(request_font_initialize_ = false);
    }
    
    // Set the font as initialized.
    inline void SetFontInitialized()
    {
        font_initialized_ = true;
        request_font_initialize_ = false;
    }

private:
    // Update the text interface.
    void updateText(ITextInterface::Attribute attribute)
    {
        if (interface_ == nullptr)
            return;

        // If the font failed to initialize the first time but the user has attempted
        // to change the font to a different path, attempt to initialize the font
        // again.
        if (FontReinitializeRequest() || attribute == ITextInterface::ATTRIBUTE_FONT_NAME)
        {
            interface_->Update(ITextInterface::ATTRIBUTE_FONT_NAME);

            // Don't update the font twice for no reason.
            if (attribute == ITextInterface::ATTRIBUTE_FONT_NAME)
                return;
        }

        // Conclude here if the font still hasn't initialized successfully.
        if (!IsFontInitialized())
            return;

        interface_->Update(attribute);
    }

public:
    // This should only be accessed by the game engine code!
    ITextInterface* interface_                  = nullptr;

private:
    std::string buffer_                         = "";
    std::string font_name_                      = "";
    Color color_                                = { 255, 255, 255, 255 };
    HorizontalAlignment horizontal_alignment_   = HORIZONTAL_LEFT;
    FontStyleFlags font_style_flags_            = FONT_STYLE_NONE;
    float font_size_                            = 12.f;
    int wrap_width_                             = 0;

    bool font_initialized_                      = false;
    bool request_font_initialize_               = true;
};

}   // namespace nuke