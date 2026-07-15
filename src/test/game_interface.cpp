// floason (C) 2026
// Licensed under the MIT License.

#include <iostream>
#include <cstdint>

#include "nuke.hpp"
#include "game_interface.hpp"
#include "testentity.hpp"

nuke::IEngine* engine;
nuke::IRenderer* renderer;

static TestGame game_ = TestGame();
TestGame* game = &game_;

static TestEntityManager entity_manager_ = TestEntityManager();
TestEntityManager* entity_manager = &entity_manager_;

static TestEntity* test4;
static TestEntity* test5;
static nuke::PixelBufferDescriptor descriptor(100, 100);

static float test4_x_base = 320;
static float test4_y_base = -240;

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

// Get the size of the game window.
nuke::Vector2 TestGame::GetSize()
{
    return { 640, 480 };   
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
}

// Called on engine initialization, if the game engine already aggregates the
// game interface.
bool TestGame::Init()
{
    engine->PrecacheImage("C:/Windows/Web/Wallpaper/Windows/img0.jpg");
    engine->PrecacheImage("E:/test.png");
    engine->PrecacheSound("C:/Windows/Media/Alarm01.wav");

    TestEntity* test = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test->render_context.render_size = { 50, 50 };
    test->collision.origin = { 100, -50 };
    test->AdjustRenderOrigin();

    TestEntity* test2 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test2->render_context.render_size = { 50, 50 };
    test2->render_context.color = { 0, 255, 0, 127 };
    test2->collision.origin = { 175, -125 };
    test2->AdjustRenderOrigin();

    TestEntity* test3 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test3->render_context.render_size = { 50, 50 };
    test3->render_context.color = { 255, 0, 0, 127 };
    test3->collision.origin = { 150, -100 };
    test3->AdjustRenderOrigin();

    test4 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test4->SetTexture(engine->LoadImage("C:/Windows/Web/Wallpaper/Windows/img0.jpg"));
    test4->render_context.render_size = { 50, 50 };
    test4->render_context.scale = true;
    test4->collision.maxs = { 25, -25 };
    test4->collision.mins = { -25, 25 };
    test4->collision.origin = { test4_x_base, test4_y_base };
    test4->AdjustRenderOrigin();

    test5 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test5->SetTexture(engine->LoadImage("E:/test.png"));
    test5->render_context.render_size = { 50, 50 };
    test5->collision.maxs = { 25, -25 };
    test5->collision.mins = { -25, 25 };
    test5->collision.origin = { 475, -275 };
    test5->AdjustRenderOrigin();

    TestEntity* test6 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test6->SetTexture(engine->CreateRawTexture("texture_stream", &descriptor));
    test6->OwnTexture();
    test6->render_context.render_size = { 100, 100 };
    test6->collision.origin = { 100, -330 };
    test6->AdjustRenderOrigin();

    TestEntity* cursor = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    cursor->render_context.render_size = { 1, 1 };
    cursor->collision.origin = { 320, -240 };
    cursor->AdjustRenderOrigin();

    nuke::ISound* sound = engine->LoadSound("C:/Windows/Media/Alarm01.wav");
    sound->Play(true);
    sound->SetParentEntity(test4);
    sound->SetMaxDistance(2000);

    // test
    delete engine->CreateRawTexture("texture_test", 3, "hi");

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
    test4->collision.origin = { test4_x_base + std::sin(commonvars.ticks / 10.f) * 50, 
                                                   test4_y_base + std::cos(commonvars.ticks / 10.f) * 50 };
    test4->AdjustRenderOrigin();

    static int frame = 0;
    static int direction = 1;
    if (commonvars.ticks % 5 == 0) 
    {
        if ((frame += direction) % 4 == 0)
            direction = -direction;
        test5->render_context.crop_offset = nuke::Vector2(frame * 50, 0);
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