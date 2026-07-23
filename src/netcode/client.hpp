// floason (C) 2026
// Licensed under the MIT License.

// Engine client netcode.

#pragma once

#include <cstdint>
#include <SDL3_net/SDL_net.h>

#include "nuke.hpp"

namespace nuke
{

class NetcodeClient : public INetcodeClient
{
// INetcodeClient
public:
    // Get the current connection status of the client.
    virtual ConnectionStatus GetStatus() override;

public:
    // Connect to a server. By default, the client will make 4 connection 
    // attempts, with 6-second intervals between each attempt, before failing.
    // This function does not block beyond resolving the hostname. The client
    // must be closed when done via IEngine::CloseClient().
    bool Connect(const char* hostname, uint16_t port, int attempts = 4, float wait = 6);


    // Called by the engine per-frame.
    bool PerFrame();

    // Called by the engine per-tick.
    bool PerTick(bool last_per_frame);

    // Close the client connection.
    void Close(ConnectionStatus new_status = STATUS_CLOSED);
    
    // Is the connection closed?
    inline bool IsClosed()
    {
        return status == STATUS_FAILED || status == STATUS_CLOSED;
    }

private:
    // Send a connection packet to the server socket.
    void sendConnectionPacket();

public:
    ConnectionStatus status             = STATUS_PENDING;

private:
    NET_DatagramSocket* local_socket_   = nullptr;
    NET_Address* host_address_          = nullptr;
    uint16_t host_port_                 = UINT_MAX;

    // Used during connection validation.
    float last_conn_packet_timestamp_   = 0.f;
    float wait_interval_                = 0.f;
    int attempts_remaining_             = 0;
};

}   // namespace nuke