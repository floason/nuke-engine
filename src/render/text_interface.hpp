// floason (C) 2026
// Licensed under the MIT License.

// Alows interactivity between a text descriptor instance and its
// respective text texture so as to update the texture's attributes.

#pragma once

#include "nuke.hpp"
#include "texture.hpp"

namespace nuke
{

class TextInterface : public ITextInterface
{
public:
    // Bind the designated text texture instance.
    TextInterface(TextureText* texture);

// ITextInterface
public:
    // Invoke the text texture to update the desired attribute on its
    // end.
    virtual bool Update(Attribute attribute) override;

private:
    TextureText* texture_   = nullptr;
};

}   // namespace nuke