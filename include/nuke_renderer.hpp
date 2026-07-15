// floason (C) 2026
// Licensed under the MIT License.

// Interface class for the engine's renderer component.

#pragma once

#include "sdk/nuke_renderable.hpp"

namespace nuke
{

class IRenderer
{
public:
    // Start tracking a new renderable instance.
    virtual RenderHandle AddRenderable(IRenderable* renderable, RenderContext& context) = 0;

    // Stop tracking a renderable instance.
    virtual void RemoveRenderable(RenderHandle handle) = 0;
};

}   // namespace nuke