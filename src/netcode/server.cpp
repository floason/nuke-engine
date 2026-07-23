// floason (C) 2026
// Licensed under the MIT License.

// Engine server netcode.

#include <cstdint>
#include <SDL3_net/SDL_net.h>

#include "nuke.hpp"
#include "engine.hpp"
#include "server.hpp"

namespace nuke
{

// Start the server. The server must be closed when done via
// IEngine::CloseServer().
bool NetcodeServer::Start(bool localhost)
{
    // Attempt to bind to the port specified by the s_listen_port game
    // variable and continue up to s_listen_port + 10 (non-inclusive) on
    // consecutive failures.
    uint16_t port = engine.s_listen_port.GetInt();
    for (int i = 0; i < 10; i++)
    {
        socket_ = NET_CreateDatagramSocket((localhost ? engine.net.localhost : nullptr), port, 0);
        if (socket_ != nullptr)
            break;
        std::cerr << "Could not listen to port " << port << "..." << std::endl;
        port++;
    }
    if (socket_ == nullptr)
    {
        std::cerr << "Failed to bind to server socket!" << std::endl;
        return false;
    }
    
    std::cout << "Listening for incoming connections on port " << port << "." << std::endl;
    return true;
}

// Shut down the server.
void NetcodeServer::Shutdown()
{
    if (socket_ != nullptr)
    {
        std::cout << "Closing server connection." << std::endl;
        NET_DestroyDatagramSocket(socket_);
    }
    engine.net.server = nullptr;
}

// Called by the engine per-frame.
bool NetcodeServer::PerFrame()
{
    // Poll for incoming packets.
    NET_Datagram* datagram = nullptr;
    for (;;)
    {
        if (!NET_ReceiveDatagram(socket_, &datagram))
            return false;
        
        if (datagram == nullptr)
            break;

        // Is this a new connection attempt?
        if (datagram->buflen == engine.net.validate_connection_buf_len
            && memcmp(datagram->buf, 
                      engine.net.validate_connection_buf, 
                      engine.net.validate_connection_buf_len) == 0)
        {
            std::cout << "new connection? version: " << &datagram->buf[4] << std::endl;
            
            // Send the exact datagram's contents back to the client to
            // signal the accepted connection on their end.
            NET_SendDatagram(socket_, datagram->addr, datagram->port, datagram->buf, datagram->buflen);
        }

        NET_DestroyDatagram(datagram);
    }
    
    return true;
}

// Called by the engine per-tick.
bool NetcodeServer::PerTick(bool last_per_frame)
{
    return true;
}

}   // namespace nuke