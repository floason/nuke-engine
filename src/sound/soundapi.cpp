// floason (C) 2026
// Licensed under the MIT License.

// This serves as a separate component from the engine for abstracting
// SDL3_mixer away from the main engine code.

#include "soundapi.hpp"

namespace nuke
{

// Initialize the sound API.
bool SoundAPI::Init()
{
    if (!MIX_Init())
        return false;

    if ((mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr)) == nullptr)
        return false;

    return true;
}

// Shut down the sound API.
void SoundAPI::Shutdown()
{
    if (mixer_ != nullptr)
        MIX_DestroyMixer(mixer_);
    MIX_Quit();
}

}   // namespace nuke