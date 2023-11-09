/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/guise-client-udp-c
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef GUISE_CLIENT_UDP_H
#define GUISE_CLIENT_UDP_H

#include <clog/clog.h>
#include <datagram-transport/transport.h>
#include <guise-client/client.h>
#include <udp-client/udp_client.h>

struct GuiseClientUdpSecret;

typedef struct UdpClientSocketInfo {
    struct UdpClientSocket* clientSocket;
} UdpClientSocketInfo;

typedef struct GuiseClientUdp {
    GuiseClient guiseClient;
    DatagramTransport transport;
    UdpClientSocketInfo socketInfo;
    UdpClientSocket socket;
    Clog log;
} GuiseClientUdp;

int guiseClientUdpInit(GuiseClientUdp* self, struct ImprintAllocator* memory, const char* name,
    uint16_t port, const struct GuiseClientUdpSecret* secret);

int guiseClientUdpUpdate(GuiseClientUdp* self, MonotonicTimeMs now);

#endif
