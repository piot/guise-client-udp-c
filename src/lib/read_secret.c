/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/guise-client-udp-c
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/text_in_stream.h>
#include <guise-client-udp/read_secret.h>
#include <guise-serialize/parse_text.h>

/// Reads secret user.txt file according to the format specified here https://github.com/piot/guise-daemon#userstxt
/// @param secret first secret found in file
/// @return negative on error, positve otherwise
int guiseClientUdpReadSecret(GuiseClientUdpSecret* secret)
{
    CLOG_DEBUG("reading secret file")
    FILE* fp = fopen("users.txt", "r");
    if (fp == 0) {
        CLOG_ERROR("could not find users.txt")
        //        return -4;
    }

    char line[1024];
    char* ptr = fgets(line, 1024, fp);
    if (ptr == 0) {
        return -39;
    }
    fclose(fp);

    FldTextInStream textInStream;
    FldInStream inStream;

    fldInStreamInit(&inStream, (const uint8_t*)line, tc_strlen(line));
    fldTextInStreamInit(&textInStream, &inStream);

    GuiseSerializeUserInfo userInfo;

    int err = guiseTextStreamReadUser(&textInStream, &userInfo);
    if (err < 0) {
        return err;
    }

    secret->userId = userInfo.userId;
    secret->passwordHash = userInfo.passwordHash;

    return 0;
}
