// floason (C) 2026
// Licensed under the MIT License.

// Defines the renderer component of the Nuke engine. This manages IRenderables 
// and is responsible for drawing them.

#include <cassert>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "vt323.hpp"

namespace nuke
{

// Start tracking a new renderable instance.
RenderHandle Renderer::AddRenderable(IRenderable* renderable, RenderContext& context)
{
    RenderHandle handle = renderables_.size();
    renderables_.emplace_back(renderable, context);
    return handle;
}

// Stop tracking a renderable instance.
void Renderer::RemoveRenderable(RenderHandle handle)
{
    assert(handle < renderables_.size() && "Attempted to remove invalid renderable handle!");

    // Rather than traverse through the renderables vector to locate the 
    // renderable, the renderable can be removed in O(1) by simply swapping 
    // it with the last renderable and calling ::pop_back() to remove the 
    // old pointer at the end of the vector.
    renderables_[handle] = renderables_.back();
    renderables_[handle].ptr->GetRenderHandle() = handle;
    renderables_.pop_back();
}

// Initialize the renderer.
bool Renderer::Init(Vector2& game_size)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        return false;

    // Before calling IGame::Init(), the SDL renderer must be set up. This
    // requires a window to be spawned, so it is hidden until the main loop is
    // started. Initiating the renderer is required for managing SDL-related
    // assets, particularly textures.
    if (!SDL_CreateWindowAndRenderer(engine.game->GetName(), 
                                     game_size.x, 
                                     game_size.y, 
                                     SDL_WINDOW_HIDDEN, 
                                     &sdl_window, 
                                     &sdl_renderer))
        return false;

    // A 16x16 "missing texture" texture is generated in place of any textures
    // that fail to load (for texture instances which inherit TextureSDL).
    Color* pixels;
    int pitch;
    missing_texture_ = SDL_CreateTexture(sdl_renderer,
                                         SDL_PIXELFORMAT_RGBA32,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         16,
                                         16);
    if (missing_texture_ == nullptr)
        return false;
    SDL_LockTexture(missing_texture_, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
    for (int y = 0; y < 16; y++)
    {
        Color* row = pixels->GetRow(pitch, y);
        for (int x = 0; x < 16; x++)
            row[x] = { static_cast<uint8_t>((x == y || (15 - x) == y) ? 255 : 0), 0, 0, 255 };
    }
    SDL_UnlockTexture(missing_texture_);

    if (!TTF_Init())
        return false;
    
    if ((sdl_text_engine = TTF_CreateRendererTextEngine(sdl_renderer)) == nullptr)
        return false;

    if ((default_font_io_stream = SDL_IOFromConstMem(vt323_ttf, vt323_ttf_len)) == nullptr)
        return false;

    return true;
}

// Prepare drawing the next frame.
void Renderer::BeginFrame()
{
    // Clear the screen.
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl_renderer);
}

// Start the renderer prior to the engine's main loop.
void Renderer::Start()
{
    // The window and renderer is created during engine initialization, but the
    // window is not made visible. Now that the engine is starting, it must 
    // finally be made visible.
    SDL_ShowWindow(sdl_window);

    // TODO: split into separate render phases so that SDL_BLENDMODE_NONE is used
    // in place for solid objects?
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
}

// Take over from the engine to draw each managed renderable.
void Renderer::DrawFrame()
{
    // Iterate over each managed renderable.
    for (const auto& renderable : renderables_)
    {
        TextureBase* texture = static_cast<TextureBase*>(renderable.context->texture);
        Vector2 centre_origin = renderable.context->origin - engine.camera_context.camera_offset;
        switch (texture->GetType())
        {
#define X(TYPE)                                                                         \
            case TextureType::TYPE:                                                     \
                static_cast<TYPE*>(texture)->Draw(centre_origin, *renderable.context);  \
                break;
            
            TEXTURE_TYPE_MACROS
#undef X

            default:
                assert(false && "Unkown texture type!");
        }
    }
}

// Update the renderer after drawing each desired renderable.
void Renderer::EndFrame()
{
    SDL_RenderPresent(sdl_renderer);
}

// Shut down the renderer.
void Renderer::Shutdown()
{
    if (missing_texture_ != nullptr)
        SDL_DestroyTexture(missing_texture_);
    if (sdl_window != nullptr)
        SDL_DestroyWindow(sdl_window);
    if (sdl_renderer != nullptr)
        SDL_DestroyRenderer(sdl_renderer);
    if (sdl_text_engine != nullptr)
        TTF_DestroyRendererTextEngine(sdl_text_engine);
    TTF_Quit();
    renderables_.clear();
}

}   // namespace nuke