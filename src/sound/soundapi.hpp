// floason (C) 2026
// Licensed under the MIT License.

// This serves as a separate component from the engine for abstracting
// SDL3_mixer away from the main engine code.

#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include "sound.hpp"

namespace nuke
{

class SoundAPI
{
    friend Sound;

public:
    // Initialize the sound API.
    bool Init();

    // Shut down the sound API.
    void Shutdown();

private:
    MIX_Mixer* mixer_   = nullptr;
};

}   // namespace nuke