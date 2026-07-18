// floason (C) 2026
// Licensed under the MIT License.

// This creates a very preliminary test game using Nuke. This code was written
// before an SDK was derived for Nuke, thus it implements its own frontends of
// the IEntity and its affiliated interfaces.

#include "nuke.hpp"
#include "game_interface.hpp"
#include "testentity.hpp"

static int engine_failed()
{
    std::cout << engine->GetLastError() << std::endl;
    engine->Shutdown();
    return EXIT_FAILURE;
}

int main(int argc, char** argv)
{
    engine = nuke::IEngine::GetEngineAPI(
        nuke::engine_major, 
        nuke::engine_minor, 
        nuke::engine_patch
    );
    if (engine == nullptr)
        return EXIT_FAILURE;

    renderer = engine->GetRenderer();
    camera = engine->GetCameraView();

    engine->SetGameInterface(game);
    engine->PrintVersion();
    if (!engine->Init(argc - 1, &argv[1])) // The first parameter is always the loaded binary path.
        return engine_failed();
    if (!engine->Start())
        return engine_failed();

    engine->Shutdown();
    return EXIT_SUCCESS;
}