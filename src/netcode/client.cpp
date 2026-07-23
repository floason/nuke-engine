// floason (C) 2026
// Licensed under the MIT License.

// Engine client netcode.

#include <SDL3_net/SDL_net.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "client.hpp"

namespace nuke
{

// Get the current connection status of the client.
INetcodeClient::ConnectionStatus NetcodeClient::GetStatus()
{
    return status;
}

// Connect to a server. By default, the client will make 4 connection 
// attempts, with 6-second intervals between each attempt, before failing.
// This function does not block beyond resolving the hostname. The client
// must be closed when done via IEngine::CloseClient().
bool NetcodeClient::Connect(const char* hostname, uint16_t port, int attempts, float wait)
{
    // Resolve the hostname to connect to.
    host_port_ = port;
    if ((host_address_ = NET_ResolveHostname(hostname)) == nullptr)
        goto fail;
    NET_WaitUntilResolved(host_address_, -1);

    // Resolve the local client's socket.
    if ((local_socket_ = NET_CreateDatagramSocket(nullptr, 0, 0)) == nullptr)
        goto fail;

    wait_interval_      = wait;
    attempts_remaining_ = attempts;
    sendConnectionPacket();

    return true;
fail:
    Close(STATUS_FAILED);
    return false;
}

// Called by the engine per-frame.
bool NetcodeClient::PerFrame()
{
    // Skip if the connection has been closed.
    if (IsClosed())
        return true;

    // Poll for incoming packets.
    NET_Datagram* datagram = nullptr;
    for (;;)
    {
        if (!NET_ReceiveDatagram(local_socket_, &datagram))
            return false;
        
        if (datagram == nullptr)
            break;

        // If we're waiting for a new connection, we have to wait for
        // the server to respond with the same connection packet first.
        if (status == STATUS_PENDING)
        {
            if (datagram->buflen == engine.net.validate_connection_buf_len
                && memcmp(datagram->buf, 
                        engine.net.validate_connection_buf, 
                        engine.net.validate_connection_buf_len) == 0)
            {
                status = STATUS_SUCCEEDED;
                engine.game->OnGameEvent(IGame::EVENT_CLIENT_SUCCESS);
            }
        }

        NET_DestroyDatagram(datagram);
    }

    // If the client still hasn't made a successful connection, check
    // whether to send a new connection packet or whether to abort the
    // connection attempt.
    if (status == STATUS_PENDING
        && engine.game->commonvars.realtime > last_conn_packet_timestamp_ + wait_interval_)
    {
        // Abort the connection if no more attempts should be made.
        if (attempts_remaining_ <= 0)
        {
            Close(STATUS_FAILED);
            return true;
        }
        else
            sendConnectionPacket();
    }

    return true;
}

// Called by the engine per-tick.
bool NetcodeClient::PerTick(bool last_per_frame)
{
    // Skip if the connection has been closed.
    if (IsClosed())
        return true;

    return true;
}

// Close the client connection.
void NetcodeClient::Close(INetcodeClient::ConnectionStatus new_status)
{
    assert(new_status >= STATUS_FAILED && "New status must be after success enum on client close!");

    if (IsClosed())
        return;

    if (host_address_ != nullptr)
        NET_UnrefAddress(host_address_);
    if (local_socket_ != nullptr)
        NET_DestroyDatagramSocket(local_socket_);
    status = new_status;
    engine.game->OnGameEvent(new_status == STATUS_FAILED 
                             ? IGame::EVENT_CLIENT_FAILED 
                             : IGame::EVENT_CLIENT_CLOSED);
}

// Send a connection packet to the server socket.
void NetcodeClient::sendConnectionPacket()
{
    NET_SendDatagram(local_socket_, 
                     host_address_, 
                     host_port_, 
                     engine.net.validate_connection_buf, 
                     engine.net.validate_connection_buf_len);

    last_conn_packet_timestamp_ = engine.game->commonvars.realtime;
    attempts_remaining_--;
}

}   // namespace nuke