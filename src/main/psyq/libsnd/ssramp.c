#include "rr/channels.h"
#include "psyq/libsnd/vmchval.h"
#include "psyq/libsnd/ssramp.h"
#include "psyq/libsnd/ssatr2.h"
void _SsSndCrescendo(s32 bankInput, s32 rowInput) {
    s16 bank = bankInput, row = rowInput;
    u16 left, right;
    s32 bankBits = (bank * 0x10000U);
    ChannelValueRecordB0 **table = _ss_score;
    ChannelValueRecordB0 **slot = (ChannelValueRecordB0 **)((u8 *)table + (bankBits >> 14) * (sizeof(*table) / sizeof(u32)));
    s32 offset = (s16)row * (s32)sizeof(ChannelValueRecordB0);
    ChannelValueRecordB0 *base = *slot;
    ChannelValueRecordB0 *record = (ChannelValueRecordB0 *)((u8 *)base + offset);
    ChannelValueRecordB0 *current;
    s32 elapsed = (record->wa0 + 1U);
    s32 duration = record->w9c;
    u32 delta;
    s32 l, r;
    s32 packed;
    record->wa0 = elapsed;
    if (duration < elapsed) {
        (*slot)[row].w98 &= ~0x10;
    } else {
        s32 product = ((u32)(s32)record->h48[0] * elapsed);
        delta = product / duration;
        delta = (delta - record->h48[1]);
        if (delta != 0) {
            packed = (s16)(bank | ((u32)row << 8));
            record->h48[1] = (u16)record->h48[1] + delta;
            _SsVmGetSeqVol(packed, &left, &right);
            l = (left + delta);
            if (l >= 128) l = 127;
            if (l < 0) l = 0;
            r = (right + delta);
            if (r >= 128) r = 127;
            if (r < 0) r = 0;
            _SsVmSetSeqVol(packed, (u16)l, (u16)r, 1);
            if ((l == 127 && r == l) || (l == 0 && r == 0)) {
                _ss_score[(s16)bank][(s16)row].w98 &= ~0x10;
            }
        }
    }
    _SsVmGetSeqVol((s16)(bank | ((u32)row << 8)), &record->h58[2], &record->h58[3]);
}
