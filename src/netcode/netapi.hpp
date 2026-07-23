// floason (C) 2026
// Licensed under the MIT License.

// Netcode API component of the engine.

#pragma once

#include <cstddef>
#include <SDL3_net/SDL_net.h>

#include "client.hpp"
#include "server.hpp"

#define NETAPI_PACKET_MAGIC     0x656B756E  // "nuke"
#define NETAPI_PACKET_MAGIC_STR "nuke"

namespace nuke
{

class NetcodeAPI
{
public:
    // Initialize the netcode API.
    bool Init();

    // Shut down the netcode API.
    void Shutdown();

public:
    NET_Address* localhost              = nullptr;
    NetcodeServer* server               = nullptr;
    NetcodeClient* client               = nullptr;

    char validate_connection_buf[128];
    size_t validate_connection_buf_len;
};

}   // namespace nuke