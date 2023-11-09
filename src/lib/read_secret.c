#include <clog/clog.h>
#include <flood/text_in_stream.h>
#include <flood/in_stream.h>

#include <guise-client-udp/read_secret.h>
#include <guise-serialize/parse_text.h>

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
