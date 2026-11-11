#include "rr/channel_stream.h"
#include "rr/audio_cd_controller.h"

extern char StrNotSeqData[];
extern char SeqOldFormatMsg[];
extern s32 _SsReadDeltaValue(s32, s32);

/* Read the 24-bit big-endian tempo (microseconds per quarter note). */
static inline s32 read_tempo(ChannelStreamB0 *c) {
    s32 b0, b1, b2;

    b0 = *(c->c0++);
    b1 = *(c->c0++);
    b2 = *(c->c0++);
    return b2 | ((b0 << 16) | (b1 << 8));
}

/* Initialise sequence slot a0 (vab id a1) from SEQ data at seq. */
s32 _SsInitSoundSeq(s32 slotInput, s32 a1, u8 *seq) {
    s16 a0 = slotInput;
    s32 i;
    u8 c;
    s32 b0, b1;
    s32 lo, hi, t;
    u32 m, v0;
    u8 *q;
    ChannelStreamB0 *s0 = *(_ss_score + a0);

    s0->b26 = a1;
    s0->h48[4] = 0;
    s0->b18 = 0;
    s0->b19 = 0;
    s0->b1e = 0;
    s0->b1a = 0;
    s0->b1b = 0;
    s0->b1f = 0;
    s0->b17 = 0;
    s0->w84 = 0;
    s0->w88 = 0;
    s0->w8c = 0;
    s0->h48[7] = 0;
    s0->b21 = 0;
    s0->b20 = 1;
    s0->b14 = 0;
    s0->w90 = 0;
    s0->b1c = 0;
    s0->b1d = 0;
    s0->b15 = 0;
    s0->b16 = 0;
    s0->h80 = 0;
    s0->p24[0] = 0;
    s0->p24[1] = 0;

    for (i = 0; i < 16; i++) {
        s0->b37[i] = i;
        s0->b27[i] = 0x40;
        s0->h60[i] = 0x7F;
    }

    s0->h48[5] = 1;
    s0->c0 = seq;
    c = *s0->c0;
    if (c == 'S' || c == 'p') {
        s0->c0 += 7;
        if (*(s0->c0++) != 1) {
            DebugPrintStub(StrNotSeqData);
            return -1;
        }
    } else {
        DebugPrintStub(SeqOldFormatMsg);
        return 0;
    }

    b0 = *(s0->c0++);
    b1 = *(s0->c0++);
    s0->h48[4] = b1 | (b0 << 8);

    t = read_tempo(s0);
    s0->w8c = t;

    lo = 0x03938700 / t;
    hi = 0x03938700 % t;
    if ((s32)((u32)t >> 1) < hi) {
        s0->w8c = lo + 1;
    } else {
        s0->w8c = lo;
    }

    s0->w94 = s0->w8c;
    s0->p24[0] = *(s0->c0++);
    s0->p24[1] = *(s0->c0++);

    s0->w90 = s0->w84 = _SsReadDeltaValue(a0, 0);
    s0->p10 = 0;
    m = (u32)(s32)s0->h48[4] * s0->w8c;
    s0->c4 = s0->c0;
    s0->c8 = s0->c0;
    s0->cC = s0->c0;
    if (m * 10 < VBLANK_MINUS * 60) {
        s0->h48[5] = (VBLANK_MINUS * 600) / m;
        s0->h48[6] = s0->h48[5];
    } else {
        v0 = ((u32)(s32)s0->h48[4] * s0->w8c * 10) / (VBLANK_MINUS * 60);
        s0->h48[5] = -1;
        s0->h48[6] = v0;
        if ((VBLANK_MINUS * 30) < ((u32)(s32)s0->h48[4] * s0->w8c * 10) % (VBLANK_MINUS * 60)) {
            s0->h48[6] = v0 + 1;
        }
    }
    s0->h48[7] = (u16)s0->h48[6];
    return 0;
}
