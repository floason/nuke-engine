// floason (C) 2026
// Licensed under the MIT License.

// Nuke engine.

#pragma once

#include <vector>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "nuke.hpp"
#include "renderer.hpp"

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

    // Initialize the engine.
    virtual bool Init() override;

    // Precache an image texture.
    virtual void PrecacheImage(const char* path) override;

    // Load a precached image texture. Returns NULL if not found.
    virtual ITexture* LoadImage(const char* path) override;

    // Precache a sound path.
    virtual void PrecacheSound(const char* path) override;

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
    
    MIX_Mixer* mixer                                                = nullptr;
    IGame* game                                                     = nullptr;
    CameraContext camera_context;
    
private:
    
    std::unordered_map<std::string, ITexture*> precached_images_;
    std::unordered_map<std::string, ISound*> precached_sounds_;
    char error_[256];

    // Keep track of updatables whose methods should be dispatched at a specified
    // time.
    std::vector<Updatable*> updatables_;
};

extern Engine engine;

}   // namespace nuke