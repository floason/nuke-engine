// floason (C) 2026
// Licensed under the MIT License.

// Defines the renderer component of the Nuke engine. This manages
// IRenderables and is responsible for drawing them.

#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "nuke.hpp"

namespace nuke
{

class Renderer : public IRenderer
{
// IRenderer
public:
    // Start tracking a new renderable instance.
    virtual RenderHandle AddRenderable(IRenderable* renderable, RenderContext& context) override;

    // Stop tracking a renderable instance.
    virtual void RemoveRenderable(RenderHandle handle) override;

public:
    // Initialize the renderer.
    bool Init(Vector2& game_size);

    // Start the renderer prior to the engine's main loop.
    void Start();

    // Take over from the engine to draw each managed renderable.
    void DrawFrame();

    // Shut down the renderer.
    void Shutdown();

public:
    // Get the "missing texture" texture.
    inline SDL_Texture* GetMissingTexture()
    {
        return missing_texture_;
    }

public:
    SDL_Renderer* sdl_renderer              = nullptr;
    SDL_Window* sdl_window                  = nullptr;
    TTF_TextEngine* sdl_text_engine         = nullptr;
    SDL_IOStream* default_font_io_stream    = nullptr;

private:
    struct Renderable
    {
        IRenderable* ptr;
        RenderContext* context;

        Renderable(IRenderable* ptr, RenderContext& context) :
            ptr(ptr),
            context(&context)
        {
        }
    };
    std::vector<Renderable> renderables_;

    SDL_Texture* missing_texture_           = nullptr;
};

}   // namespace nuke