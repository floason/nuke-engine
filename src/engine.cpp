// floason (C) 2026
// Licensed under the MIT License.

// Nuke engine.

#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "sound.hpp"
#include "texture.hpp"

namespace nuke
{
    
Engine engine = Engine();

// Retrieve the singleton Nuke engine instance.
IEngine* IEngine::GetEngineAPI(int major, int minor, int patch)
{
    if (major != nuke::engine_major || minor != nuke::engine_minor || patch != nuke::engine_patch)
        return nullptr;
    return &engine;
}

// Get the renderer interface.
IRenderer* Engine::GetRenderer()
{
    return &renderer;
}

// Get the camera view.
CameraContext* Engine::GetCameraView()
{
    return &camera_context;
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
    // If an SDL error was thrown, this is simply just buffered into the engine 
    // instance's error_ buffer.
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

    Vector2 size = engine.game->GetSize();
    if (!renderer.Init(size))
        return false;

    if (!MIX_Init())
        return false;

    if ((mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr)) == nullptr)
        return false;

    camera_context.attenuation_offset = Vector2(size.x / 2.f, -size.y / 2.f);
    if (!game->Init())
        return false;

    return true;
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

// Precache a sound path.
void Engine::PrecacheSound(const char* path)
{
    precached_sounds_[path] = new Sound(path);
}

// Create a raw sound instance utilising a pre-existing float signed sample
// PCM buffer for the audio data, alongside frequency and channel information.
// The buffer must be alive for the entire duration of this sound instance.
ISound* Engine::CreateRawSound(const void* data, size_t len, int channels, int frequency)
{
    return new Sound(data, len, channels, frequency);
}

// Load a precached sound. Because the same sound instance will always be
// returned, the track used for the returned sound will be the same, meaning
// playing the sound while it is already playing will just restart it, rather
// than play both sounds simultaneously.
ISound* Engine::LoadSound(const char* path)
{
    return precached_sounds_[path];
}

// Copy a sound instance to create a new track. This sound instance does not
// copy the base sound's underlying audio data, therefore the base sound must
// be alive for the entire duration of this copy! Returns NULL if the base
// sound instance did not load successfully.
ISound* Engine::CopySound(ISound* other, bool free_after_play)
{
    Sound* copy = new Sound(*static_cast<Sound*>(other));
    if (!copy->Loaded())
        return nullptr;
    if (free_after_play)
        copy->DestroyOnFinish();
    return copy;
}

// Dispatch an updatable's invokation at a later time period.
void Engine::DispatchUpdate(Updatable* updatable, float time_of_dispatch)
{
    assert(updatable != nullptr && "Updatable must not be nullptr.");

    updatable->time_of_dispatch = time_of_dispatch;
    updatables_.push_back(updatable);
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

    CommonVars& vars = game->GetCommonVars();
    vars.tick_interval = 1.f / game->GetTickRate();
    if (vars.tick_interval <= 0.f)
    {
        SetErrorBuffer("Game tickrate must be > 0!");
        return false;
    }

    renderer.Start();

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
                // Invoke the game interface's ::PerTick() method.
                if (!game->PerTick(i + 1 == count))
                    break;

                // Iterate through each updatable and dispatch their set update
                // functions when desired. It is possible that an updatable may
                // re-insert itself during dispatch; these should not be
                // processed now.
                std::vector<Updatable*> to_traverse;
                to_traverse.swap(updatables_);
                for (int obj_i = 0, size = to_traverse.size(); obj_i < size; obj_i++)
                {
                    Updatable* updatable = to_traverse[obj_i];
                    if (vars.curtime >= updatable->time_of_dispatch)
                        updatable->Update();
                    else
                        updatables_.push_back(updatable);
                }
                
                vars.ticks++;
                vars.curtime += vars.tick_interval;
            }
            last_tick_timestamp += game_tick_interval * count;
        }

        // Invoke the game interface's ::PerFrame() method.
        if (!game->PerFrame())
            break;

        // Interface with the renderer.
        renderer.DrawFrame();

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
        vars.realtime += vars.frametime;
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
    if (mixer != nullptr)
        MIX_DestroyMixer(mixer);
    renderer.Shutdown();
    MIX_Quit();
    SDL_Quit();
    return true;
}

// Create a raw texture instance with an optional parameter. Ideally, this should
// be checked for NULL on failure before an entity takes ownership of the created 
// texture, as the returned texture instance will otherwise not be managed!
ITexture* Engine::createRawTexture(const char* texture_name, void* optional)
{
    return TextureFactoryBase::Create(texture_name, optional);
}

// Set the engine error buffer.
void Engine::SetErrorBuffer(const char* error)
{
    std::strncpy(error_, error, sizeof(error_));
}

}   // namespace nuke