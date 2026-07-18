// floason (C) 2026
// Licensed under the MIT License.

// Interface class for defining game information and callback methods.

#pragma once

#include "nuke_commonvars.hpp"
#include "nuke_math.hpp"
#include "nuke_macros.hpp"

namespace nuke
{

class IEngine;

class IGame
{
public:
    // Get a reference to the game's common variables struct.
    virtual CommonVars& GetCommonVars() = 0;

    // Get the name of the game.
    virtual const char* GetName() = 0;

    // Get the size of the game window.
    virtual Vector2 GetSize() = 0;

    // Return the tickrate used for simulating physics and logic.
    virtual float GetTickRate() { return NUKE_DEFAULT_TICKRATE; }

    // Called when the game interface has been attached to the engine.
    virtual void OnEngineAttach() { }

    // Called on engine initialization, if the game engine already aggregates the
    // game interface.
    virtual bool Init() { return true; }

    // Per-frame method which is invoked by the engine after starting.
    virtual bool PerFrame() { return true; }

    // Per-tick method which is invoked by the engine after starting.
    virtual bool PerTick(bool last_per_frame) { return true; }

    // Called when the process exits.
    virtual bool Exit(bool window_closed) { return true; }
};

}   // namespace nuke