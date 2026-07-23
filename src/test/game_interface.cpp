// floason (C) 2026
// Licensed under the MIT License.

#include <iostream>
#include <cstdint>

#include "nuke.hpp"
#include "game_interface.hpp"
#include "testentity.hpp"

nuke::IEngine* engine;
nuke::IRenderer* renderer;
nuke::INetcodeClient* client;
nuke::INetcodeServer* server;
nuke::CameraContext* camera;

static TestGame game_ = TestGame();
TestGame* game = &game_;

static TestEntityManager entity_manager_ = TestEntityManager();
TestEntityManager* entity_manager = &entity_manager_;

static TestEntity* test4;
static TestEntity* test5;
static TestEntity* funny_entity = nullptr;
static TestEntity* test7;
static nuke::PixelBufferDescriptor descriptor(100, 100);
static nuke::TextDescriptor text("");

static float test4_x_base = 320;
static float test4_y_base = -240;

// Used for testing event invokation.
class TestEventListener : public nuke::IEventListener
{
public:
    TestEventListener()
    {
        engine->AddEventListener(this, "the most event ever");
        engine->AddEventListener(this, "the least event ever");
    }

    ~TestEventListener()
    {
        engine->RemoveEventListener(this);
    }

// nuke::IEventListener
public:
    // Called when an event is signalled.
    virtual void OnSignalEvent(nuke::IEvent* event) override
    {
        std::cout << "EVENT SIGNALLED! " << event->GetName() << std::endl;
    }
};

static TestEventListener* listener;

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

// Called on engine initialization, if the game engine already aggregates the
// game interface.
bool TestGame::Init()
{
    engine->PrecacheImage("C:/Windows/Web/Wallpaper/Windows/img0.jpg");
    engine->PrecacheImage("E:/test.png");
    engine->PrecacheSound("C:/Windows/Media/Alarm01.wav");
    engine->PrecacheSound("heavy_scram2012_falling01.mp3");
    engine->PrecacheFont("C:/Windows/Fonts/arial.ttf");
    engine->PrecacheFont("C:/Windows/Fonts/sserife.fon", "ms sans serif");

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
    test4->collision.origin = { test4_x_base - 100, test4_y_base + 100 };
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

    test7 = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
    test7->SetTexture(engine->CreateRawTexture("texture_text", &text));
    test7->OwnTexture();
    text.SetFont("C:/Windows/Fonts/arial.ttf");
    text.SetBuffer("Hello World! I Am Text!");
    text.SetFontSize(12);
    text.SetWrapWidth(50);
    text.SetHorizontalWrapAlignment(nuke::TextDescriptor::HORIZONTAL_CENTRE);
    test7->AdjustBounds();
    test7->collision.origin = { 50, -50 }; // You can change horizontal/vertical alignment using mins/maxs.
    test7->AdjustRenderOrigin();

    // test
    delete engine->CreateRawTexture("texture_test", 3, "hi");

    listener = new TestEventListener();

    return true;
}

// Called once when the engine starts.
bool TestGame::Start()
{
    nuke::ISound* sound = engine->LoadSound("C:/Windows/Media/Alarm01.wav");
    sound->Play(true);
    sound->SetPlaybackSpeed(1.02);
    sound->SetParentEntity(test4);
    sound->SetMaxDistance(2000);

    // preliminary networking test
    if ((server = engine->StartServer(true)) == nullptr)
        return false;
    if ((client = engine->ConnectClient("localhost", 32775)) == nullptr)
        return false;

    return true;
}

// Per-frame method which is invoked by the engine after starting. It is
// recommended that game logic is placed in the ::PerTick() method instead.
bool TestGame::PerFrame()
{
    if (commonvars.realtime > 15)
    {
        camera->camera_offset.x = std::sin((commonvars.realtime - 10) * 5) * 50;
        camera->camera_offset.y = std::cos((commonvars.realtime - 10) * 5) * 50;
    }

    return true;
}

// Per-tick method which is invoked by the engine after starting.
bool TestGame::PerTick(bool last_per_frame)
{
    if (commonvars.curtime > 5)
    {
        test4->collision.origin = { test4_x_base + std::sin(commonvars.ticks / 10.f) * 50, 
                                    test4_y_base + std::cos(commonvars.ticks / 10.f) * 50 };
        test4->AdjustRenderOrigin();
    }

    if (funny_entity == nullptr && commonvars.curtime >= 8 && commonvars.curtime < 12)
    {
        funny_entity = static_cast<TestEntity*>(entity_manager->CreateEntity("test_entity"));
        funny_entity->render_context.render_size = { 100, 100 };
        funny_entity->collision.origin = { -3000, -350 };
        funny_entity->collision.velocity = { 4000, 0 };
        funny_entity->collision.maxs = { 50, -50 };
        funny_entity->collision.mins = { -50, 50 };

        nuke::ISound* sound = engine->LoadSound("heavy_scram2012_falling01.mp3");
        sound->Play(true);
        sound->SetVolume(4.f);
        sound->SetParentEntity(funny_entity);
        sound->SetMaxDistance(3000);
    }
    if (funny_entity != nullptr)
    {
        funny_entity->collision.origin.x += funny_entity->collision.velocity.x * commonvars.tick_interval;
        funny_entity->AdjustRenderOrigin();

        if (commonvars.curtime >= 12)
        {
            std::cout << "deleting funny entity" << std::endl;

            delete funny_entity;
            funny_entity = nullptr;
        }
    }

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

    static int color_r = 0;
    static int color_direction = 4;
    color_r += color_direction;
    if (color_r > 255)
    {
        color_r = 255 - (color_r - 255);
        color_direction = -5;
    }
    else if (color_r < 0)
    {
        color_r = -color_r;
        color_direction = 5;
    }
    text.SetColor({ (uint8_t)color_r, 0, 0, 255 });

    if (commonvars.ticks == NUKE_DEFAULT_TICKRATE * 5)
        text.SetFont("ms sans serif");
    else if (commonvars.ticks == NUKE_DEFAULT_TICKRATE * 8)
        text.SetFont("");
    else if (commonvars.ticks == NUKE_DEFAULT_TICKRATE * 10)
        text.SetFontSize(25.f);
    else if (commonvars.ticks == NUKE_DEFAULT_TICKRATE * 12)
        text.SetFontStyleFlags(nuke::TextDescriptor::FONT_STYLE_BOLD);

    if (commonvars.ticks == NUKE_DEFAULT_TICKRATE * 5)
    {
        nuke::IEvent* event = engine->CreateEvent("the most event ever");
        engine->FireEvent(event);
    }

    if (commonvars.ticks == NUKE_DEFAULT_TICKRATE * 7)
    {
        nuke::IEvent* event = engine->CreateEvent("the least event ever");
        engine->FireEvent(event);
    }

    return true;
}

// Invoked by the engine when a specific game event is signalled. This
// is separate to the IEvent interface.
void TestGame::OnGameEvent(GameEvent event)
{
    switch (event)
    {
        case EVENT_CLIENT_SUCCESS:
            std::cout << "successful connection!" << std::endl;
            break;
        case EVENT_CLIENT_FAILED:
            std::cout << "client connection failed!" << std::endl;
            break;
        case EVENT_CLIENT_CLOSED:
            std::cout << "client connection closed!" << std::endl;
            break;
        default:
            std::cerr << "unknown event " << event << "?" << std::endl;
            break;
    }
}

// Called when the process exits.
bool TestGame::Exit(bool window_closed)
{
    std::cout << "Game is exiting!" << std::endl;
    std::cout << "window_closed: " << window_closed << std::endl;
    return true;
}