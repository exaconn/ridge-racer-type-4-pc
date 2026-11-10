/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
#include "rr/sequencer_init.h"
#include "psyq/libsnd/vmtick.h"
#include "psyq/libsnd/sstick3.h"

#include "psyq/libsnd/vmkoff.h"

void _SsSndReplay(s32 bankInput, s16 rowInput);
void _SsSndStop(s32 bankInput, s32 rowInput);

void _SsSndReplay(s32 bankInput, s16 rowInput) {
    u32 bankBits = (u32)bankInput << 16;
    SequenceChannel **table = _ss_score;
    SequenceChannel **bank = &table[(s32)bankBits >> 16];
    SequenceChannel *record = &(*bank)[rowInput];
    record->b14 = 1;
    (*bank)[(s16)rowInput].w98 &= ~8;
}

void _SsSndStop(s32 bankInput, s32 rowInput) {
    u32 bankBits = (u32)bankInput << 16;
    SequenceChannel **table = _ss_score;
    SequenceChannel **bank = &table[(s32)bankBits >> 16];
    s32 rowBits;
    SequenceChannel *record = &(*bank)[(s16)rowInput];
    s32 i;
    SequenceChannel *liveRecord;
    record->w98 &= ~1;
    (*bank)[(s16)rowInput].w98 &= ~2;
    rowBits = (u32)rowInput << 8;
    (*bank)[(s16)rowInput].w98 &= ~8;
    /* This numeric row-address expression remains reconstruction debt. */
    liveRecord = (SequenceChannel *)((s16)rowInput * (s32)sizeof(SequenceChannel) + (unsigned long)*bank);
    liveRecord->w98 &= ~0x400;
    (*bank)[(s16)rowInput].w98 |= 4;
    _SsVmSeqKeyOff((s16)(bankInput | rowBits));
    _SsVmDamperOff();
    record->b14 = 0;
    record->w88 = 0;
    record->b1c = 0;
    record->b18 = 0;
    record->b19 = 0;
    record->b1e = 0;
    record->b1a = 0;
    record->b1b = 0;
    record->b1f = 0;
    record->b17 = 0;
    record->b21 = 0;
    record->b1c = 0;
    record->b1d = 0;
    record->b15 = 0;
    record->b16 = 0;
    record->w90 = record->w84;
    record->w94 = record->w8c;
    record->h48[6] = record->h48[7];
    record->c0 = record->c4;
    record->c8 = record->c4;
    for (i = 0; i < 0x10; i++) {
        record->b37[i] = i;
        record->b27[i] = 0x40;
        record->h60[i] = 0x7F;
    }
    record->h58[2] = 0x7F;
    record->h58[3] = 0x7F;
}
