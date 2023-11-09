/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/guise-client-udp-c
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <guise-client-udp/client.h>
#include <guise-client-udp/read_secret.h>
#include <udp-client/udp_client.h>

static int udpClientSocketInfoSend(void* _self, const uint8_t* data, size_t size)
{
    UdpClientSocketInfo* self = (UdpClientSocketInfo*)_self;

    return udpClientSend(self->clientSocket, data, size);
}

static ssize_t udpClientSocketInfoReceive(void* _self, uint8_t* data, size_t size)
{
    UdpClientSocketInfo* self = (UdpClientSocketInfo*)_self;

    return udpClientReceive(self->clientSocket, data, size);
}

int guiseClientUdpInit(GuiseClientUdp* self, struct ImprintAllocator* memory, const char* name,
    uint16_t port, const GuiseClientUdpSecret* secret)
{
    self->transport.receive = udpClientSocketInfoReceive;
    self->transport.send = udpClientSocketInfoSend;
    self->transport.self = &self->socketInfo;

    udpClientInit(&self->socket, name, port);

    self->socketInfo.clientSocket = &self->socket;

    Clog guiseClientLog;
    guiseClientLog.config = &g_clog;
    guiseClientLog.constantPrefix = "GuiseClient";

    guiseClientInit(&self->guiseClient, memory, guiseClientLog);
    guiseClientReInit(&self->guiseClient, &self->transport, secret->userId, secret->passwordHash);
    //GuiseClientState reportedState = GuiseClientStateIdle;

    return 0;
}

int guiseClientUdpUpdate(GuiseClientUdp* self, MonotonicTimeMs now)
{
    return guiseClientUpdate(&self->guiseClient, now);
}
