// floason (C) 2026
// Licensed under the MIT License.

// Test texture for debugging purposes.

#include <tuple>
#include <cassert>

#include "nuke.hpp"
#include "texture_test.hpp"

namespace nuke
{

using params = std::tuple<int, const char*>;
REGISTER_TEXTURE("texture_test", params, TextureTest);

// Test texture.
TextureTest::TextureTest(int a, const char* b)
{
    type_ = TextureType::TextureTest;
    std::cout << "INT: " << a << std::endl;
    std::cout << "CONST CHAR*: " << b << std::endl;
}

// This texture cannot be drawn!
void TextureTest::Draw(Vector2 origin, RenderContext& context)
{
    assert(false);
}

}   // namespace nuke