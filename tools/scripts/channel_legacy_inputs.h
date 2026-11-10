#ifndef RR_TEST_CHANNEL_LEGACY_INPUTS_H
#define RR_TEST_CHANNEL_LEGACY_INPUTS_H
/* Frozen native input layouts from the selector-only batch. These describe
 * existing test corpora, not production channel objects or ABI aliases. */
#include "rr/channels.h"

typedef struct LegacyChannelStatusViewInput {
    u8 pad00[4];
    const ChannelGateEntry5 *entries04;
    u8 pad08[4];
    s32 reset0C;
    struct LegacyChannelStatusViewInput *owner10;
    ChannelUpdateCallback update14;
    u8 pad18[0x24-0x18], argument24, pad25[3];
    u8 *data28;
    u8 *data2C;
    u8 *input30;
    u8 length34,pad35, length36, command37, pad38[4];
    u8 *packet;
    u8 *buffer40;
    u8 b44, b45;
    u8 phase;
    u8 state47, pad48, state49, pad4A[0x50-0x4A], b50, pad51[0x57-0x51], gates57[6], payload5D[6], pad63[0xE4-0x63], argumentE4, padE5;
    u16 pending;
} LegacyChannelStatusViewInput;

typedef struct {
    LegacyChannelStatusViewInput status;
    u8 responseE8,entry_countE9,padEA[6];
} LegacyChannelRecordF0Input;

typedef struct {
    u8 pad00[0x28];
    u8 *data28, *data2C;
    u8 pad30[4], length34, pad35, length36, command37, previous38;
    u8 pad39[12], phase45, pad46[14], gates54[9];
} LegacyChannelTransferViewInput;

typedef struct {
    u8 pad00[0x14];
    ChannelUpdateCallback update;
    ChannelStatusCallback complete;
    u8 pad1C[0x2C-0x1C];
    u8 *data2C;
    u8 pad30[6], length36, command37, pad38[0x46-0x38];
    u8 phase, pad47[0x51-0x47], first, second, equal;
    u8 pad54[0x5D-0x54], payload[6], pad63[0xE4-0x63], argument;
} LegacyChannelPatchCommandInput;
#endif
