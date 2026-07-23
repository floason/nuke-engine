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
    enum GameEvent
    {
        // Sent when a networked client makes a successful connection.
        EVENT_CLIENT_SUCCESS,

        // Sent when a networked client fails to make a successful connection.
        EVENT_CLIENT_FAILED,

        // Sent when a networked client closes its connection.
        EVENT_CLIENT_CLOSED
    };

public:
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

    // Called once when the engine starts.
    virtual bool Start() { return true; }

    // Per-frame method which is invoked by the engine after starting. It is
    // recommended that game logic is placed in the ::PerTick() method instead.
    virtual bool PerFrame() { return true; }

    // Per-tick method which is invoked by the engine after starting.
    virtual bool PerTick(bool last_per_frame) { return true; }

    // Invoked by the engine when a specific game event is signalled. This
    // is separate to the IEvent interface.
    virtual void OnGameEvent(GameEvent event) { }

    // Called when the process exits.
    virtual bool Exit(bool window_closed) { return true; }

public:
    CommonVars commonvars;
};

}   // namespace nuke