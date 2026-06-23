// floason (C) 2026
// Licensed under the MIT License.

// Interface class for the Nuke engine.

#pragma once

#include <any>

#include "nuke_macros.hpp"

namespace nuke
{

class IGame;
class ITexture;
class IEntityManager;

class IEngine
{
public:
    // Retrieve the singleton Nuke engine instance.
    NUKE_API static IEngine* GetEngineAPI(int major, int minor, int patch);

public:
    // Set the entity manager interface.
    virtual void SetEntityManager(IEntityManager* manager) = 0;

    // Print the engine version string.
    virtual void PrintVersion() = 0;
    
    // Get the last engine error.
    virtual const char* GetLastError() = 0;

    // Clear the last engine error;
    virtual void ClearLastError() = 0;

    // Aggregate the game interface instance.
    virtual void SetGameInterface(IGame* game) = 0;

    // Initialize the engine.
    virtual bool Init() = 0;

    // Create a raw texture instance with an optional parameter. Ideally, this should
    // be checked for NULL on failure before an entity takes ownership of the created 
    // texture, as the returned texture instance will otherwise not be managed!
    virtual ITexture* CreateRawTexture(const char* texture_name, std::any optional = std::any()) = 0;

    // Precache an image texture.
    virtual void PrecacheImage(const char* path) = 0;

    // Load a precached image texture.
    virtual ITexture* LoadImage(const char* path) = 0;

    // Start the engine and call into the game interface. This should be called only
    // after initialising the engine. This must be called from the main thread. This
    // method will block.
    virtual bool Start() = 0;

    // Shut down the engine. This should be called on process exit, including
    // on engine init failure.
    virtual bool Shutdown() = 0;
};

}   // namespace nuke