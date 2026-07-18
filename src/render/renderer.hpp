// floason (C) 2026
// Licensed under the MIT License.

// Defines the renderer component of the Nuke engine. This manages
// IRenderables and is responsible for drawing them.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "nuke.hpp"

namespace nuke
{

class Renderer : public IRenderer
{
    friend TextureText;

// IRenderer
public:
    // Start tracking a new renderable instance.
    virtual RenderHandle AddRenderable(IRenderable* renderable, RenderContext& context) override;

    // Stop tracking a renderable instance.
    virtual void RemoveRenderable(RenderHandle handle) override;

public:
    // Initialize the renderer.
    bool Init(Vector2& game_size);

    // Precache a font file.
    bool PrecacheFont(const char* path, const char* as = nullptr, bool overwrite = false);

    // Precache a font file from a byte buffer.
    bool PrecacheFont(const unsigned char* buffer, size_t length, const char* as, bool overwrite = false);

    // Start the renderer prior to the engine's main loop.
    void Start();

    // Prepare drawing the next frame.
    void BeginFrame();

    // Take over from the engine to draw each managed renderable.
    void DrawFrame();

    // Update the renderer after drawing each desired renderable.
    void EndFrame();

    // Shut down the renderer.
    void Shutdown();

public:
    // Get the "missing texture" texture.
    inline SDL_Texture* GetMissingTexture()
    {
        return missing_texture_;
    }

public:
    SDL_Renderer* sdl_renderer                                      = nullptr;
    SDL_Window* sdl_window                                          = nullptr;
    TTF_TextEngine* sdl_text_engine                                 = nullptr;

private:
    std::unordered_map<std::string, TTF_Font*> precached_fonts_;

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