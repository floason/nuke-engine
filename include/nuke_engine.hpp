// floason (C) 2026
// Licensed under the MIT License.

// Interface class for the Nuke engine.

#pragma once

#include <any>
#include <tuple>

#include "nuke_macros.hpp"
#include "nuke_camera.hpp"
#include "nuke_gamevar.hpp"
#include "nuke_event.hpp"

namespace nuke
{

class IRenderer;
class IGame;
class ITexture;
class ISound;
class Updatable;

class IEngine
{
public:
    // Retrieve the singleton Nuke engine instance.
    NUKE_API static IEngine* GetEngineAPI(int major, int minor, int patch);

public:
    // Get the renderer interface.
    virtual IRenderer* GetRenderer() = 0;
    
    // Get the camera view.
    virtual CameraContext* GetCameraView() = 0;

    // Print the engine version string.
    virtual void PrintVersion() = 0;
    
    // Get the last engine error.
    virtual const char* GetLastError() = 0;

    // Clear the last engine error;
    virtual void ClearLastError() = 0;

    // Aggregate the game interface instance.
    virtual void SetGameInterface(IGame* game) = 0;

    // Initialize the engine. Command-line arguments may be passed to this method
    // so as to parse any game variable modifications. The first argument must be
    // after the program name argument!
    virtual bool Init(int argc = 0, char** argv = nullptr) = 0;

    // Precache an image texture.
    virtual bool PrecacheImage(const char* path, 
                               const char* as = nullptr, 
                               bool overwrite = false) = 0;

    // Load a precached image texture.
    virtual ITexture* LoadImage(const char* path) = 0;

    // Precache a sound file.
    virtual bool PrecacheSound(const char* path, 
                               const char* as = nullptr, 
                               bool overwrite = false) = 0;

    // Create a raw sound instance utilising a pre-existing float signed sample
    // PCM buffer for the audio data, alongside frequency and channel information.
    // The buffer must be alive for the entire duration of this sound instance.
    virtual ISound* CreateRawSound(const void* data, size_t len, int channels, int frequency) = 0;

    // Load a precached sound. Because the same sound instance will always be
    // returned, the track used for the returned sound will be the same, meaning
    // playing the sound while it is already playing will just restart it, rather
    // than play both sounds simultaneously.
    virtual ISound* LoadSound(const char* path) = 0;

    // Copy a sound instance to create a new track. This sound instance does not
    // copy the base sound's underlying audio data, therefore the base sound must
    // be alive for the entire duration of this copy! Returns NULL if the base
    // sound instance did not load successfully.
    virtual ISound* CopySound(ISound* other, bool free_after_play = true) = 0;

    // Precache a font file.
    virtual bool PrecacheFont(const char* path, 
                              const char* as = nullptr, 
                              bool overwrite = false) = 0;

    // Precache a font file from a byte buffer.
    virtual bool PrecacheFont(const unsigned char* buffer, 
                              size_t length, 
                              const char* as, 
                              bool overwrite = false) = 0;

    // Install a created base game variable so that it can be searchable.
    virtual bool InstallGameVar(GameVarBase* var) = 0;

    // Find a game variable by name.
    virtual GameVar* FindGameVar(const char* name) = 0;

    // Find a game command by name.
    virtual GameCmd* FindGameCmd(const char* name) = 0;

    // Create a new event.
    virtual IEvent* CreateEvent(const char* name) = 0;

    // Fire an event object. This will delete the event object afterwards.
    virtual void FireEvent(IEvent* event) = 0;

    // Add an event listener for a given event name.
    virtual void AddEventListener(IEventListener* listener, const char* name) = 0;

    // Remove an event listener. Leave name as nullptr to stop listening to every
    // single event.
    virtual bool RemoveEventListener(IEventListener* listener, const char* name = nullptr) = 0;

    // Dispatch an updatable's invokation at a later time period.
    virtual void DispatchUpdate(Updatable* updatable, float time_of_dispatch = 0.f) = 0;

    // Start the engine and call into the game interface. This should be called only
    // after initialising the engine. This must be called from the main thread. This
    // method will block.
    virtual bool Start() = 0;

    // Shut down the engine. This should be called on process exit, including
    // on engine init failure.
    virtual bool Shutdown() = 0;

private:
    // Create a raw texture instance with an optional parameter. This is only used
    // internally.
    virtual ITexture* createRawTexture(const char* texture_name, void* optional = nullptr) = 0;

public:
    // Create a raw texture instance with an optional parameters. Ideally, this should
    // be checked for NULL on failure before an entity takes ownership of the created 
    // texture, as the returned texture instance will otherwise not be managed!
    template <typename T>
    inline ITexture* CreateRawTexture(const char* texture_name, T arg)
    {
        return createRawTexture(texture_name, &arg);
    }

    // Create a raw texture instance with the desired parameters. Ideally, this should
    // be checked for NULL on failure before an entity takes ownership of the created 
    // texture, as the returned texture instance will otherwise not be managed!
    template <typename... Args>
    inline ITexture* CreateRawTexture(const char* texture_name, Args&&... args)
    {
        auto container = std::make_tuple(std::forward<Args>(args)...);
        return createRawTexture(texture_name, &container);
    }
};

}   // namespace nuke