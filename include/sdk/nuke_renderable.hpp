// floason (C) 2026
// Licensed under the MIT License.

// Interface class for a renderable object. This is associated with entities,
// but is propagated to the engine's renderer which should not have any
// information of the entity's description or its management.

// Renderables must additionally propagate a RenderContext struct to the
// renderer, which describes information about the renderable's texture
// being drawn.

#pragma once

#include "nuke_math.hpp"
#include "nuke_color.hpp"
#include "nuke_texture.hpp"

namespace nuke
{

class IRenderer;

using RenderHandle = size_t;
inline constexpr RenderHandle InvalidRenderHandle = static_cast<RenderHandle>(~0);

struct RenderContext
{
    ITexture* texture       = nullptr;
    Vector2 render_size     = { 0, 0 };
    Vector2 crop_offset     = { 0, 0 };
    Vector2 origin          = { 0, 0 };
    Color color             = { 255, 255, 255, 255 };
    bool scale              = false;

    RenderContext() = default;
};

class IRenderable
{
public:
    virtual ~IRenderable() = default;

public:
    // Propagate this renderable to the renderer.
    virtual void AddToRenderList(RenderContext& context) = 0;

    // Tell the renderer to stop tracking this renderer.
    virtual void RemoveFromRenderList() = 0;

    // Set ownership of the loaded texture to this renderable, if applicable.
    virtual void OwnTexture() = 0;

    // Get a reference to the renderable's handle.
    virtual RenderHandle& GetRenderHandle() = 0;
};

}   // namespace nuke