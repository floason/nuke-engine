// floason (C) 2026
// Licensed under the MIT License.

// Nuke engine.

#pragma once

#include <unordered_map>
#include <SDL3/SDL.h>

#include "nuke.hpp"

namespace nuke
{

class Engine : public IEngine
{
public:
    // Print the engine version string.
    virtual void PrintVersion() override;
    
    // Get the last engine error.
    virtual const char* GetLastError() override;

    // Clear the last engine error;
    virtual void ClearLastError() override;

    // Set the entity manager interface.
    virtual void SetEntityManager(IEntityManager* manager) override;

    // Aggregate the game interface instance.
    virtual void SetGameInterface(IGame* game) override;

    // Initialize the engine.
    virtual bool Init() override;
    
    // Create a raw texture instance with an optional parameter. Ideally, this should
    // be checked for NULL on failure before an entity takes ownership of the created 
    // texture, as the returned texture instance will otherwise not be managed!
    virtual ITexture* CreateRawTexture(const char* texture_name, std::any optional = std::any()) override;

    // Precache an image texture.
    virtual void PrecacheImage(const char* path) override;

    // Load a precached image texture. Returns NULL if not found.
    virtual ITexture* LoadImage(const char* path) override;

    // Start the engine and call into the game interface. This should be called only
    // after initialising the engine. This must be called from the main thread. This
    // method will block.
    virtual bool Start() override;

    // Shut down the engine. This should be called on process exit, including
    // on engine init failure.
    virtual bool Shutdown() override;

public:
    // Set the engine error buffer.
    void SetErrorBuffer(const char* error);

    // Get the "missing texture" texture.
    SDL_Texture* GetMissingTexture();

public:
    SDL_Renderer* renderer                                          = nullptr;
    SDL_Window* window                                              = nullptr;
    IEntityManager* entity_manager                                  = nullptr;
    IGame* game                                                     = nullptr;
    
private:
    SDL_Texture* missing_texture_                                   = nullptr;
    std::unordered_map<std::string, ITexture*> precached_images_;
    char error_[256];
};

extern Engine engine;

}   // namespace nuke