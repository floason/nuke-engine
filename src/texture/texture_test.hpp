// floason (C) 2026
// Licensed under the MIT License.

// Test texture for debugging purposes.

#pragma once

#include "nuke.hpp"
#include "texture_base.hpp"

namespace nuke
{

class TextureTest : public TextureBase
{
public:
    // Test texture.
    TextureTest(int a, const char* b);

public:
    // This texture cannot be drawn!
    void Draw(Vector2 origin, RenderContext& context);
};

}   // namespace nuke