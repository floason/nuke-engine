// floason (C) 2026
// Licensed under the MIT License.

// Engine server netcode.

#pragma once

#include <SDL3_net/SDL_net.h>

#include "nuke.hpp"

namespace nuke
{

class NetcodeServer : public INetcodeServer
{
public:
    // Start the server.
    bool Start(bool localhost = false);

    // Called by the engine per-frame.
    bool PerFrame();

    // Called by the engine per-tick.
    bool PerTick(bool last_per_frame);

    // Shut down the server.
    void Shutdown();

private:
    NET_DatagramSocket* socket_ = nullptr;
};

}   // namespace nuke