// floason (C) 2026
// Licensed under the MIT License.

// Nuke engine.

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <SDL3/SDL.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "texture_base.hpp"
#include "texture_stream.hpp"

namespace nuke
{
    
Engine engine = Engine();

// Retrieve the singleton Nuke engine instance.
IEngine* IEngine::GetEngineAPI(int major, int minor, int patch)
{
    if (major != nuke::engine_major || minor != nuke::engine_minor || patch != nuke::engine_patch)
        return NULL;
    return &engine;
}

// Print the engine version string.
void Engine::PrintVersion()
{
    std::cout 
        << "Nuke engine v" 
        << nuke::engine_major 
        << "." 
        << nuke::engine_minor 
        << "." 
        << nuke::engine_patch 
        << std::endl;
}

// Get the last engine error.
const char* Engine::GetLastError()
{
    // If an SDL error was thrown, this is simply just buffered into the engine instance's
    // error_ buffer.
    const char* sdl_error = SDL_GetError();
    if (std::strlen(sdl_error) > 0)
    {
        SDL_ClearError();
        std::snprintf(error_, sizeof(error_), "SDL error: %s", sdl_error);
    }

    // Always return the engine instance's error_ buffer.
    return error_;
}

// Clear the last engine error;
void Engine::ClearLastError()
{
    std::memset(error_, 0, sizeof(error_));
}

// Set the entity manager interface.
void Engine::SetEntityManager(IEntityManager* manager)
{
    this->entity_manager = manager;
}

// Aggregate the game interface instance.
void Engine::SetGameInterface(IGame* game)
{
    this->game = game;
    game->OnEngineAttach();
}

// Initialize the engine.
bool Engine::Init()
{
    if (game == nullptr)
    {
        SetErrorBuffer("Please call Engine::SetGameInterface() to link your game instance to the " \
            "engine!");
        return false;
    }
    
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        return false;

    if (!SDL_CreateWindowAndRenderer(game->GetName(), 640, 480, SDL_WINDOW_HIDDEN, &window, &renderer))
        return false;

    if (!game->Init())
        return false;
    return true;
}

// Create a raw texture instance with an optional parameter. Ideally, this should
// be checked for NULL on failure before an entity takes ownership of the created 
// texture, as the returned texture instance will otherwise not be managed!
ITexture* Engine::CreateRawTexture(const char* texture_name, std::any optional)
{
    return TextureFactoryBase::Create(texture_name, optional);
}

// Precache an image texture.
void Engine::PrecacheImage(const char* path)
{
    precached_images_[path] = CreateRawTexture("texture_image", path);
}

// Load a precached image texture. Returns NULL if not found.
ITexture* Engine::LoadImage(const char* path)
{
    auto texture = precached_images_.find(path);
    if (texture == precached_images_.end())
        return nullptr;
    return texture->second;
}

// Start the engine and call into the game interface. This should be called only
// after initialising the engine. This must be called from the main thread. This
// method will block.
bool Engine::Start()
{
    if (game == nullptr)
    {
        SetErrorBuffer("Please call Engine::SetGameInterface() to link your game instance to the "      \
            "engine!");
        return false;
    }

    if (entity_manager == nullptr)
    {
        SetErrorBuffer("Please call Engine::SetEntityManager() to link your entity manager instance "   \
            "to the engine!");
        return false;
    }

    CommonVars& vars = game->GetCommonVars();
    vars.tick_interval = 1.f / game->GetTickRate();
    if (vars.tick_interval <= 0.f)
    {
        SetErrorBuffer("Game tickrate must be > 0!");
        return false;
    }

    // The window and renderer is created during engine initialization, but the
    // window is not made visible. Now that the engine is starting, it must 
    // finally be made visible.
    SDL_ShowWindow(window);

    // TODO: split into separate render phases so that SDL_BLENDMODE_NONE is used
    // in place for solid objects?
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // After successful initialisation, the main loop is invoked, which blocks
    // until the window is exited.
    uint64_t last_tick_timestamp = SDL_GetTicksNS();
    uint64_t game_tick_interval = (uint64_t)(vars.tick_interval * NUKE_NS_PER_S);
    uint64_t start = SDL_GetTicksNS();
    for (;;)
    {
        // Poll for any new events before invoking the game interface.
        SDL_Event event;
        bool window_closed = true;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    window_closed = false;
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    return game->Exit(window_closed);
            }
        }

        // Handle physics/logic on each individual tick.
        uint64_t current_tick = SDL_GetTicksNS();
        uint64_t frametime = current_tick - last_tick_timestamp;
        if (frametime >= game_tick_interval)
        {
            // If the framerate is behind the constant tickrate, all tick-related
            // functions must be repeated so as to maintain the constant tickrate.
            unsigned count = math::min((int)(frametime / game_tick_interval), NUKE_MAX_TICKS_PER_FRAME);
            for (unsigned i = 0; i < count; i++)
            {
                if (!game->PerTick(i + 1 == count))
                    break;
                vars.ticks++;
            }
            last_tick_timestamp += game_tick_interval * count;
        }

        // Clear the screen.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Interface with the game itself.
        if (!game->PerFrame())
            break;

        // Render each entity's texture in its designated origin.
        IEntity* entity = nullptr;
        while ((entity = entity_manager->GetEntity(entity)) != nullptr)
        {
            // TODO: z-indexes

            ITextureDescriptor* texture_descriptor = entity->GetTextureDescriptor();
            if (texture_descriptor == nullptr)
                continue;

            IPhysicsDescriptor* physics = entity->GetPhysicsDescriptor();
            if (physics == nullptr)
                continue;

            TextureBase* texture = static_cast<TextureBase*>(texture_descriptor->GetTexture());
            if (texture == nullptr)
                continue;

            // If the texture is a TextureStream instance and its pixel buffer
            // has been recently updated, it should be copied to the GPU.
            if (texture->GetType() == TextureType::STREAM)
            {
                TextureStream* stream = static_cast<TextureStream*>(texture);
                if (stream->descriptor_ != nullptr && stream->descriptor_->buffer->Ready())
                    SDL_UnlockTexture(stream->Get());
            }

            texture->Draw(physics->GetOrigin(), 
                          texture_descriptor->GetRenderSize(),
                          texture_descriptor);
        }

        // Update the screen.
        SDL_RenderPresent(renderer);

        // Delay the thread until the next frame if requested.
        current_tick = SDL_GetTicksNS();
        frametime = current_tick - start;
        float game_frametime_f = game->GetFpsMax();
        if (game_frametime_f > 0.f)
        {
            uint64_t game_frametime = (uint64_t)((1.f / game_frametime_f) * NUKE_NS_PER_S);
            if (game_frametime > frametime)
                SDL_DelayPrecise(game_frametime - frametime);
        }

        // Update frame-specific common variables information.
        current_tick = SDL_GetTicksNS();
        frametime = current_tick - start;
        vars.frametime = ((float)frametime / NUKE_NS_PER_S);
        vars.curtime += vars.frametime;
        vars.frames++;

        // Set the start timestamp for the next interval.
        start = current_tick;
    }

    return false;
}

// Shut down the engine. This should be called on process exit, including
// on engine init failure.
bool Engine::Shutdown()
{
    if (engine.window != nullptr)
        SDL_DestroyWindow(engine.window);
    if (engine.renderer != nullptr)
        SDL_DestroyRenderer(engine.renderer);
    SDL_Quit();
    return true;
}

// Set the engine error buffer.
void Engine::SetErrorBuffer(const char* error)
{
    std::strncpy(error_, error, sizeof(error_));
}

}   // namespace nuke