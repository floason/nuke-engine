// floason (C) 2026
// Licensed under the MIT License.

// Netcode API component of the engine.

#include <cstdio>
#include <SDL3_net/SDL_net.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "netapi.hpp"

namespace nuke
{

// Initialize the netcode API.
bool NetcodeAPI::Init()
{
    if (!NET_Init())
        return false;

    if ((localhost = NET_ResolveHostname("localhost")) == nullptr)
        return false;
    NET_WaitUntilResolved(localhost, -1);

    // Used as the first transmitted packet to validate a new connection
    // attempt.
    validate_connection_buf_len = snprintf(validate_connection_buf, 
                                           sizeof(validate_connection_buf), 
                                           "%s%d.%d.%d",
                                           NETAPI_PACKET_MAGIC_STR, 
                                           engine_major, 
                                           engine_minor, 
                                           engine_patch) + 1;

    return true;
}

// Shut down the netcode API.
void NetcodeAPI::Shutdown()
{
    if (localhost != nullptr)
        NET_UnrefAddress(localhost);
    NET_Quit();
}

}   // namespace nuke