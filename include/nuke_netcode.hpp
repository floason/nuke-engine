// floason (C) 2026
// Licensed under the MIT License.

// Exposes engine interfaces for client/server connections.

#pragma once

#include <cstdint>

namespace nuke
{

// Server netcode interface.
class INetcodeServer
{
public:
    virtual ~INetcodeServer() = default;
};

// Client netcode interface.
class INetcodeClient
{
public:
    enum ConnectionStatus
    {
        STATUS_PENDING,
        STATUS_SUCCEEDED,
        STATUS_FAILED,
        STATUS_CLOSED
    };

public:
    virtual ~INetcodeClient() = default;

public:
    // Get the current connection status of the client.
    virtual ConnectionStatus GetStatus() = 0;
};

}   // namespace nuke