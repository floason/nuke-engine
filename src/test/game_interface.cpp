// floason (C) 2026
// Licensed under the MIT License.

#include <iostream>
#include <cstdint>

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
static nuke::PixelBufferDescriptor descriptor(100, 100);

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

    TestEntity* test6 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test6->GetTextureDescriptor()->SetTexture(
        engine->CreateRawTexture("texture_stream", &descriptor));
    test6->GetTextureDescriptor()->OwnTexture();
    test6->GetTextureDescriptor()->GetRenderSize() = { 100, 100 };
    test6->GetPhysicsDescriptor()->GetOrigin() = { 50, -330 };

    return true;
}

// Per-frame method which is invoked by the engine after starting.
bool TestGame::PerFrame()
{
    if (commonvars.frametime > 10)
    {
        nuke::Vector2& camera = engine->GetCameraOrigin();
        camera.x = std::sin((commonvars.frametime - 10) * 5) * 50;
        camera.y = std::cos((commonvars.frametime - 10) * 5) * 50;
    }

    return true;
}

// Per-tick method which is invoked by the engine after starting.
bool TestGame::PerTick(bool last_per_frame)
{
    test4->GetPhysicsDescriptor()->GetOrigin() = { 250 + std::sin(commonvars.ticks / 10.f) * 50, 
                                                  -250 + std::cos(commonvars.ticks / 10.f) * 50 };

    static int frame = 0;
    static int direction = 1;
    if (commonvars.ticks % 5 == 0) 
    {
        if ((frame += direction) % 4 == 0)
            direction = -direction;
        test5->GetTextureDescriptor()->GetCropOffset() = nuke::Vector2(frame * 50, 0);
    }

    nuke::Color* pixel_buffer = descriptor.buffer->Lock();
    int pitch = descriptor.buffer->Pitch();
    for (unsigned y = 0, height = descriptor.GetHeight(); y < height; y++)
    {
        nuke::Color* row = pixel_buffer->GetRow(pitch, y);
        for (unsigned x = 0, width = descriptor.GetWidth(); x < width; x++)
        {
            unsigned i = (x - commonvars.ticks) * 10;
            row[x] = { (uint8_t)i, 
                       0, 
                       0,
                       (uint8_t)(255 - x * (255.f / width)), };
        }
    }
    descriptor.buffer->Unlock();

    return true;
}

// Called when the process exits.
bool TestGame::Exit(bool window_closed)
{
    std::cout << "Game is exiting!" << std::endl;
    std::cout << "window_closed: " << window_closed << std::endl;
    return true;
}