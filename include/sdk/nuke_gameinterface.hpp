// floason (C) 2026
// Licensed under the MIT License.

// Interface class for defining game information and callback methods.

#pragma once

#include "nuke_commonvars.hpp"

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

    // Return the tickrate used for simulating physics and logic.
    virtual float GetTickRate() = 0;

    // Return the maximum framerate used for processing the game (0 for unlimited).
    virtual float GetFpsMax() = 0;

    // Called when the game interface has been attached to the engine.
    virtual void OnEngineAttach() = 0;

    // Called on engine initialization, if the game engine already aggregates the
    // game interface.
    virtual bool Init() = 0;

    // Per-frame method which is invoked by the engine after starting.
    virtual bool PerFrame() = 0;

    // Per-tick method which is invoked by the engine after starting.
    virtual bool PerTick(bool last_per_frame) = 0;

    // Called when the process exits.
    virtual bool Exit(bool window_closed) = 0;
};

}   // namespace nuke