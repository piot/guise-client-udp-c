#ifndef GUISE_CLIENT_UDP_READ_SECRET_H
#define GUISE_CLIENT_UDP_READ_SECRET_H

#include <guise-serialize/types.h>

typedef struct GuiseClientUdpSecret {
    GuiseSerializeUserId userId;
    uint64_t passwordHash;
} GuiseClientUdpSecret;

int guiseClientUdpReadSecret(GuiseClientUdpSecret* secret);

#endif
