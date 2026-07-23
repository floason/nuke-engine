// floason (C) 2026
// Licensed under the MIT License.

#pragma once

#include "nuke.hpp"

class TestEntityManager;

class TestGame : public nuke::IGame
{
public:
    // Get the name of the game.
    virtual const char* GetName() override;

    // Get the size of the game window.
    virtual nuke::Vector2 GetSize() override;

    // Called on engine initialization, if the game engine already aggregates the
    // game interface.
    virtual bool Init() override;
    
    // Called once when the engine starts.
    virtual bool Start() override;

    // Per-frame method which is invoked by the engine after starting. It is
    // recommended that game logic is placed in the ::PerTick() method instead.
    virtual bool PerFrame() override;

    // Per-tick method which is invoked by the engine after starting.
    virtual bool PerTick(bool last_per_frame) override;

    // Invoked by the engine when a specific game event is signalled. This
    // is separate to the IEvent interface.
    virtual void OnGameEvent(GameEvent event) override;
    
    // Called when the process exits.
    virtual bool Exit(bool window_closed) override;
};

extern nuke::IEngine* engine;
extern nuke::IRenderer* renderer;
extern nuke::INetcodeClient* client;
extern nuke::INetcodeServer* server;
extern nuke::CameraContext* camera;
extern TestGame* game;
extern TestEntityManager* entity_manager;