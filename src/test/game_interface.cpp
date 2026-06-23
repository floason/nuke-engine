// floason (C) 2026
// Licensed under the MIT License.

#include <iostream>

#include "nuke.hpp"
#include "game_interface.hpp"
#include "testentity.hpp"

nuke::IEngine* engine;

static TestGame game_ = TestGame();
TestGame* game = &game_;

static TestEntityManager entity_manager_ = TestEntityManager();
TestEntityManager* entity_manager = &entity_manager_;

static TestEntity* test4;
static TestEntity* test5;

// Get a reference to the game's common variables struct.
nuke::CommonVars& TestGame::GetCommonVars()
{
    return commonvars;
}

// Get the name of the game.
const char* TestGame::GetName()
{
    return "NUKE ENGINE REPO - TEST GAME";
}

// Return the tickrate used for simulating physics.
float TestGame::GetTickRate()
{
    return NUKE_DEFAULT_TICKRATE;
}

// Return the maximum framerate used for processing the game (0 for unlimited).
float TestGame::GetFpsMax()
{
    return 0.f;
}

// Called when the game interface has been attached to the engine.
void TestGame::OnEngineAttach()
{
    engine->SetEntityManager(entity_manager);
}

// Called on engine initialization, if the game engine already aggregates the
// game interface.
bool TestGame::Init()
{
    engine->PrecacheImage("C:/Windows/Web/Wallpaper/Windows/img0.jpg");
    engine->PrecacheImage("E:/test.png");

    TestEntity* test = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test->GetTextureDescriptor()->GetRenderSize() = { 50, 50 };
    test->GetPhysicsDescriptor()->GetOrigin() = { 100, -50 };

    TestEntity* test2 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test2->GetTextureDescriptor()->GetRenderSize() = { 50, 50 };
    test2->GetTextureDescriptor()->GetColor() = { 255, 0, 0, 127 };
    test2->GetPhysicsDescriptor()->GetOrigin() = { 150, -100 };

    TestEntity* test3 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test3->GetTextureDescriptor()->GetRenderSize() = { 50, 50 };
    test3->GetTextureDescriptor()->GetColor() = { 0, 255, 0, 127 };
    test3->GetPhysicsDescriptor()->GetOrigin() = { 175, -125 };

    test4 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test4->GetTextureDescriptor()->SetTexture(
        engine->LoadImage("C:/Windows/Web/Wallpaper/Windows/img0.jpg"));
    test4->GetTextureDescriptor()->GetRenderSize() = { 50, 50 };
    test4->GetTextureDescriptor()->GetScale() = true;
    test4->GetPhysicsDescriptor()->GetOrigin() = { 250, -250 };

    test5 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test5->GetTextureDescriptor()->SetTexture(
        engine->LoadImage("E:/test.png"));
    test5->GetTextureDescriptor()->GetRenderSize() = { 50, 50 };
    test5->GetPhysicsDescriptor()->GetOrigin() = { 450, -250 };

    return true;
}

// Per-frame method which is invoked by the engine after starting.
bool TestGame::PerFrame()
{
    return true;
}

// Per-tick method which is invoked by the engine after starting.
bool TestGame::PerTick(bool last_per_frame)
{
    static float count = 0;
    test4->GetPhysicsDescriptor()->GetOrigin() = { 250 + std::sinf(count / 10) * 50, 
                                                  -250 + std::cosf(count / 10) * 50 };
    count++;

    static int frame = 0;
    static int direction = 1;
    if (commonvars.ticks % 5 == 0) 
    {
        if ((frame += direction) % 4 == 0)
            direction = -direction;
        test5->GetTextureDescriptor()->GetCropOffset() = nuke::Vector2(frame * 50, 0);
    }

    return true;
}