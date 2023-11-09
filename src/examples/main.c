/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/guise-client-udp-c
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/console.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <flood/text_in_stream.h>
#include <guise-client-udp/client.h>
#include <guise-client-udp/read_secret.h>
#include <guise-client/client.h>
#include <guise-serialize/debug.h>
#include <guise-serialize/parse_text.h>
#include <imprint/default_setup.h>
#include <stdio.h>
#include <udp-client/udp_client.h>
#include <unistd.h>

clog_config g_clog;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    g_clog.log = clog_console;
    g_clog.level = CLOG_TYPE_VERBOSE;

    CLOG_OUTPUT("guise client cli")

    ImprintDefaultSetup memory;
    imprintDefaultSetupInit(&memory, 16 * 1024 * 1024);

    int startupErr = udpClientStartup();
    if (startupErr < 0) {
        return startupErr;
    }

    const char* hostToConnectTo = "127.0.0.1";
    uint16_t portToConnectTo = 27004;
    if (argc > 1) {
        hostToConnectTo = argv[1];
        if (argc > 2) {
            char* endptr;
            portToConnectTo = strtol(argv[2], &endptr, 10);
            if (*endptr != 0) {
                return -44;
            }
        }
    }
    GuiseClientUdp guiseClientUdp;

    GuiseClientUdpSecret secret;
    guiseClientUdpReadSecret(&secret);

    ImprintAllocator* allocator = &memory.tagAllocator.info;

    Clog guiseClientLog;
    guiseClientLog.config = &g_clog;
    guiseClientLog.constantPrefix = "GuiseCLI";

    guiseClientUdpInit(&guiseClientUdp, allocator, hostToConnectTo, portToConnectTo, &secret);
    GuiseClientState reportedState;
    reportedState = GuiseClientStateIdle;

    while (true) {
        usleep(16 * 1000);

        MonotonicTimeMs now = monotonicTimeMsNow();
        guiseClientUdpUpdate(&guiseClientUdp, now);

        if (reportedState != guiseClientUdp.guiseClient.state) {
            reportedState = guiseClientUdp.guiseClient.state;
            CLOG_VERBOSE("changed state to : %d", reportedState)
        }
    }

    imprintDefaultSetupDestroy(&memory);
}
