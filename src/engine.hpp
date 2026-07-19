// floason (C) 2026
// Licensed under the MIT License.

// Nuke engine.

#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "nuke.hpp"
#include "texture.hpp"
#include "event.hpp"
#include "renderer.hpp"
#include "soundapi.hpp"

namespace nuke
{

// Engine class which incorporates the IEngine interface.
class Engine : public IEngine
{
// IEngine
public:
    // Get the renderer interface.
    virtual IRenderer* GetRenderer() override;

    // Get the camera view.
    virtual CameraContext* GetCameraView() override;

    // Print the engine version string.
    virtual void PrintVersion() override;
    
    // Get the last engine error.
    virtual const char* GetLastError() override;

    // Clear the last engine error;
    virtual void ClearLastError() override;

    // Aggregate the game interface instance.
    virtual void SetGameInterface(IGame* game) override;

    // Initialize the engine. Command-line arguments may be passed to this method
    // so as to parse any game variable modifications. The first argument must be
    // after the program name argument!
    virtual bool Init(int argc = 0, char** argv = nullptr) override;

    // Precache an image texture.
    virtual bool PrecacheImage(const char* path, 
                               const char* as = nullptr, 
                               bool overwrite = false) override;

    // Load a precached image texture. Returns NULL if not found.
    virtual ITexture* LoadImage(const char* path) override;

    // Precache a sound path.
    virtual bool PrecacheSound(const char* path, 
                               const char* as = nullptr, 
                               bool overwrite = false) override;

    // Create a raw sound instance utilising a pre-existing float signed sample
    // PCM buffer for the audio data, alongside frequency and channel information.
    // The buffer must be alive for the entire duration of this sound instance.
    virtual ISound* CreateRawSound(const void* data, size_t len, int channels, int frequency) override;

    // Load a precached sound. Because the same sound instance will always be
    // returned, the track used for the returned sound will be the same, meaning
    // playing the sound while it is already playing will just restart it, rather
    // than play both sounds simultaneously.
    virtual ISound* LoadSound(const char* path) override;

    // Copy a sound instance to create a new track. This sound instance does not
    // copy the base sound's underlying audio data, therefore the base sound must
    // be alive for the entire duration of this copy! Returns NULL if the base
    // sound instance did not load successfully.
    virtual ISound* CopySound(ISound* other, bool free_after_play = true) override;

    // Precache a font file.
    virtual bool PrecacheFont(const char* path, 
                              const char* as = nullptr, 
                              bool overwrite = false) override;

    // Precache a font file from a byte buffer.
    virtual bool PrecacheFont(const unsigned char* buffer, 
                              size_t length, 
                              const char* as, 
                              bool overwrite = false) override;

    // Install a created base game variable so that it can be searchable.
    virtual bool InstallGameVar(GameVarBase* var) override;

    // Find a game variable by name.
    virtual GameVar* FindGameVar(const char* name) override;

    // Find a game command by name.
    virtual GameCmd* FindGameCmd(const char* name) override;

    // Create a new event.
    virtual IEvent* CreateEvent(const char* name) override;

    // Fire an event object. This will delete the event object afterwards.
    virtual void FireEvent(IEvent* event) override;

    // Add an event listener for a given event name.
    virtual void AddEventListener(IEventListener* listener, const char* name) override;

    // Remove an event listener. Leave name as nullptr to stop listening to every
    // single event.
    virtual bool RemoveEventListener(IEventListener* listener, const char* name = nullptr) override;

    // Dispatch an updatable's invokation at a later time period.
    virtual void DispatchUpdate(Updatable* updatable, float time_of_dispatch = 0.f) override;

    // Start the engine and call into the game interface. This should be called only
    // after initialising the engine. This must be called from the main thread. This
    // method will block.
    virtual bool Start() override;

    // Shut down the engine. This should be called on process exit, including
    // on engine init failure.
    virtual bool Shutdown() override;

// IEngine
private:
    // Create a raw texture instance with an optional parameter. This is only used
    // internally.
    virtual ITexture* createRawTexture(const char* texture_name, void* optional = nullptr) override;

public:
    // Set the engine error buffer.
    void SetErrorBuffer(const char* error);

public:
    Renderer renderer;
    SoundAPI sound;
    CameraContext camera_context;
    IGame* game                                                         = nullptr;
    
private:
    std::unordered_map<std::string, ITexture*> precached_images_;
    std::unordered_map<std::string, ISound*> precached_sounds_;
    std::unordered_map<std::string, GameVarBase*> precached_gamevars_;
    std::unordered_map<std::string, std::unordered_set<IEventListener*>> event_manager_;
    char error_[256];

    // Keep track of updatables whose methods should be dispatched at a specified
    // time.
    std::vector<Updatable*> updatables_;

    TextureText* fps_counter_                                           = nullptr;
    TextDescriptor fps_counter_descriptor_;
    RenderContext fps_counter_render_context_;

    GameVar fps_max_                = GameVar("fps_max", 0.f, "Framerate cap (set to 0 to disable)", 1.f);
    GameVar max_ticks_per_frame_    = GameVar("max_ticks_per_frame", NUKE_MAX_TICKS_PER_FRAME, "", 1);
    GameVar show_fps                = GameVar("show_fps", false);
};

extern Engine engine;

}   // namespace nuke