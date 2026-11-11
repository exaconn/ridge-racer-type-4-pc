#include "psyq/libpad/padstop.h"
#include "rr/channel_advance.h"

s32 _padChannelToPort(void *channel) {
    s32 i = 0;
    s32 port = 0x10;
    PadChannel *record = PadChannelRecordArray;

    for (; i < 2; i++) {
        if (channel == record) return port;
        port += 0x10;
        record++;
    }
    return 0xFF;
}

void *_padPortToChannel(s32 selector) {
    PadChannel *record = PadChannelRecordArray;

    if (selector & 0xF0) record++;
    return record;
}
