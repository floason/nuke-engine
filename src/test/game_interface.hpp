// floason (C) 2026
// Licensed under the MIT License.

#pragma once

#include "nuke.hpp"

class TestEntityManager;

class TestGame : public nuke::IGame
{
public:
    // Get a reference to the game's common variables struct.
    virtual nuke::CommonVars& GetCommonVars() override;

    // Get the name of the game.
    virtual const char* GetName() override;

    // Get the size of the game window.
    virtual nuke::Vector2 GetSize() override;

    // Return the tickrate used for simulating physics and logic.
    virtual float GetTickRate() override;

    // Return the maximum framerate used for processing the game (0 for unlimited).
    virtual float GetFpsMax() override;

    // Called when the game interface has been attached to the engine.
    virtual void OnEngineAttach() override;

    // Called on engine initialization, if the game engine already aggregates the
    // game interface.
    virtual bool Init() override;

    // Per-frame method which is invoked by the engine after starting.
    virtual bool PerFrame() override;

    // Per-tick method which is invoked by the engine after starting.
    virtual bool PerTick(bool last_per_frame) override;
    
    // Called when the process exits.
    virtual bool Exit(bool window_closed) override;

public:
    nuke::CommonVars commonvars;
    float fps = 0.f;
};

extern nuke::IEngine* engine;
extern TestGame* game;
extern TestEntityManager* entity_manager;