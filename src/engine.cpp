// floason (C) 2026
// Licensed under the MIT License.



#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "sound.hpp"
#include "event.hpp"
#include "renderer.hpp"
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
        std::snprintf(error_, sizeof(error_), "SDL error: %s", sdl_error);
        SDL_ClearError();
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

// Initialize the engine. Command-line arguments may be passed to this method
// so as to modify any game variables or 
bool Engine::Init(int argc, char** argv)
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

    fps_counter_ = static_cast<TextureText*>(CreateRawTexture("texture_text", &fps_counter_descriptor_));
    fps_counter_render_context_.origin = { 10.f, -5.f };
    fps_counter_descriptor_.SetBuffer("fps:");
    fps_counter_descriptor_.SetFontSize(15.f);
    fps_counter_descriptor_.SetColor({ 127, 127, 127, 255 });
    fps_counter_descriptor_.SetFontStyleFlags(TextDescriptor::FONT_STYLE_BOLD);

    InstallGameVar(&fps_max_);
    InstallGameVar(&max_ticks_per_frame_);
    InstallGameVar(&show_fps);

    // If any command-line arguments (from argv[1] onwards) were provided,
    // these should be parsed so as to allow for the modification of game
    // variables.
    //
    // Syntax: --gvar1 value1 --gvar2 value2 ...
    GameVar* variable = nullptr;
    for (int i = 0; i < argc; i++)
    {
        // Stop early if the indexed argument is nullptr.
        if (argv[i] == nullptr)
            break;

        // Locate the game variable first. Hidden game variables cannot be 
        // modified.
        if (variable == nullptr)
        {
            if (std::strlen(argv[i]) <= 2
                || std::strncmp(argv[i], "--", 2) != 0 
                || (variable = FindGameVar(&argv[i][2])) == nullptr
                || variable->GetFlags() & GameVarBase::FLAG_HIDDEN)
            {
                std::cout << "Invalid variable name: " << argv[i] << std::endl;
            }
            continue;
        }
        
        // Attempt to update the game variable's value.
        switch (variable->GetType())
        {
            case GameVar::TYPE_INT:
            {
                variable->SetInt(std::stoi(argv[i]));
                break;
            }

            case GameVar::TYPE_FLOAT:
            {
                variable->SetFloat(std::stof(argv[i]));
                break;
            }

            case GameVar::TYPE_STRING:
            {
                variable->SetString(argv[i]);
                break;
            }
        }
        variable = nullptr;
    }
    if (variable != nullptr)
    {
        std::cout << "Missing value while attempting to modify game variable: " << variable->GetName() <<
                     std::endl;
    }

    return true;
}

// Precache an image texture.
bool Engine::PrecacheImage(const char* path, const char* as, bool overwrite)
{
    if (precached_images_.find(path) != precached_images_.end() && !overwrite)
        return false;

    if (as == nullptr)
        as = path;
    return (precached_images_[as] = CreateRawTexture("texture_image", path)) != nullptr;
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
bool Engine::PrecacheSound(const char* path, const char* as, bool overwrite)
{
    if (precached_sounds_.find(path) != precached_sounds_.end() && !overwrite)
        return false;
    
    if (as == nullptr)
        as = path;
    return (precached_sounds_[as] = new Sound(path)) != nullptr;
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

// Precache a font file.
bool Engine::PrecacheFont(const char* path, const char* as, bool overwrite)
{
    return renderer.PrecacheFont(path, as, overwrite);
}

// Precache a font file from a byte buffer.
bool Engine::PrecacheFont(const unsigned char* buffer, size_t length, const char* as, bool overwrite)
{
    return renderer.PrecacheFont(buffer, length, as, overwrite);
}

// Install a created base game variable so that it can be searchable.
bool Engine::InstallGameVar(GameVarBase* var)
{
    if (var == nullptr)
        return false;
    if (precached_gamevars_.find(var->GetName()) != precached_gamevars_.end())
        return false;
    precached_gamevars_[var->GetName()] = var;
    return true;
}

// Find a game variable by name.
GameVar* Engine::FindGameVar(const char* name)
{
    if (precached_gamevars_.find(name) == precached_gamevars_.end())
        return nullptr;

    // Ensure that the indexed base game variable is not callable before
    // returning it.
    GameVarBase* gamevar = precached_gamevars_[name];
    if (gamevar->IsCallable())
        return nullptr;

    return static_cast<GameVar*>(gamevar);
}

// Find a game command by name.
GameCmd* Engine::FindGameCmd(const char* name)
{
    if (precached_gamevars_.find(name) == precached_gamevars_.end())
        return nullptr;

    // Ensure that the indexed base game variable is callable before
    // returning it.
    GameVarBase* gamevar = precached_gamevars_[name];
    if (!gamevar->IsCallable())
        return nullptr;

    return static_cast<GameCmd*>(gamevar);
}

// Create a new event.
IEvent* Engine::CreateEvent(const char* name)
{
    return new Event(name);
}

// Fire an event object. This will delete the event object afterwards.
void Engine::FireEvent(IEvent* event)
{
    if (event == nullptr)
        return;
    
    // Check whether any listeners are listening to this event.
    const char* name = event->GetName();
    for (auto& listener : event_manager_)
    {
        if (listener.second.find(name) != listener.second.end())
            listener.first->OnSignalEvent(event);
    }

    delete event;
}

// Add an event listener for a given event name.
void Engine::AddEventListener(IEventListener* listener, const char* name)
{
    if (event_manager_.find(listener) == event_manager_.end())
        event_manager_[listener] = std::unordered_set<std::string>();
    if (event_manager_[listener].find(name) == event_manager_[listener].end())
        event_manager_[listener].insert(name);
}

// Remove an event listener.
bool Engine::RemoveEventListener(IEventListener* listener, const char* name)
{
    if (event_manager_.find(listener) == event_manager_.end())
        return false;

    if (name == nullptr)
    {
        event_manager_.erase(listener);
        return true;
    }

    if (event_manager_[listener].find(name) == event_manager_[listener].end())
        return false;

    event_manager_[listener].erase(name);
    return true;
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
            unsigned count = math::min((int)(frametime / game_tick_interval), 
                max_ticks_per_frame_.GetInt());
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
        renderer.BeginFrame();
        renderer.DrawFrame();

        // Render the FPS counter after the rendering stage so as to draw it on top
        // of everything else.
        if (show_fps.GetInt())
        {
            fps_counter_descriptor_.SetBuffer(std::string("fps: ") + std::to_string((int)vars.fps));
            fps_counter_->Draw(fps_counter_render_context_.origin, fps_counter_render_context_);
        }

        // Update the screen.
        renderer.EndFrame();

        // Delay the thread until the next frame if requested.
        current_tick = SDL_GetTicksNS();
        frametime = current_tick - start;
        float game_frametime_f = fps_max_.GetFloat();
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
        
        // Calculate the smoothed displayable framerate variable.
        if (vars.frametime > 0.f)
        {
            float t = 1.f - std::expf(-vars.frametime);
            vars.fps = nuke::math::lerp(vars.fps, 1.f / vars.frametime, t);
        }

        // Set the start timestamp for the next interval.
        start = current_tick;
    }

    return false;
}

// Shut down the engine. This should be called on process exit, including
// on engine init failure.
bool Engine::Shutdown()
{
    if (fps_counter_ != nullptr)
        delete fps_counter_;
    renderer.Shutdown();

    for (auto& texture : precached_images_)
    {
        if (texture.second != nullptr)
            delete texture.second;
    }
    for (auto& sound : precached_sounds_)
    {
        if (sound.second != nullptr)
            delete sound.second;
    }

    precached_images_.clear();
    precached_sounds_.clear();
    precached_gamevars_.clear();
    event_manager_.clear();

    if (mixer != nullptr)
        MIX_DestroyMixer(mixer);
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