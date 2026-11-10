/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channel_stream.h"
#include "rr/audio_cd_controller.h"

extern char StrNotSeqData[];
extern char SeqOldFormatMsg[];
extern s32 _SsReadDeltaValue(s32, s32);

/* Read the 24-bit big-endian tempo (microseconds per quarter note). */
static inline s32 read_tempo(ChannelStreamB0 *c) {
    s32 b0, b1, b2;

    b0 = *(c->cursor00++);
    b1 = *(c->cursor00++);
    b2 = *(c->cursor00++);
    return b2 | ((b0 << 16) | (b1 << 8));
}

/* Initialise sequence slot a0 (vab id a1) from SEQ data at seq. */
s32 _SsInitSoundSeq(s16 a0, s32 a1, u8 *seq) {
    s32 i;
    u8 c;
    s32 b0, b1;
    s32 lo, hi, t;
    u32 m, v0;
    u8 *q;
    ChannelStreamB0 *s0 = *(_ss_score + a0);

    s0->b26 = a1;
    s0->scale50 = 0;
    s0->b18 = 0;
    s0->b19 = 0;
    s0->b1e = 0;
    s0->b1a = 0;
    s0->b1b = 0;
    s0->b1f = 0;
    s0->b17 = 0;
    s0->w84 = 0;
    s0->accumulator88 = 0;
    s0->reserved8C = 0;
    s0->h56 = 0;
    s0->elapsed21 = 0;
    s0->duration20 = 1;
    s0->active14 = 0;
    s0->accumulator90 = 0;
    s0->state1C = 0;
    s0->b1d = 0;
    s0->b15 = 0;
    s0->b16 = 0;
    s0->h80 = 0;
    s0->b24 = 0;
    s0->b25 = 0;

    for (i = 0; i < 16; i++) {
        s0->b37[i] = i;
        s0->b27[i] = 0x40;
        s0->h60[i] = 0x7F;
    }

    s0->delay52 = 1;
    s0->cursor00 = seq;
    c = *s0->cursor00;
    if (c == 'S' || c == 'p') {
        s0->cursor00 += 7;
        if (*(s0->cursor00++) != 1) {
            DebugPrintStub(StrNotSeqData);
            return -1;
        }
    } else {
        DebugPrintStub(SeqOldFormatMsg);
        return 0;
    }

    b0 = *(s0->cursor00++);
    b1 = *(s0->cursor00++);
    s0->scale50 = b1 | (b0 << 8);

    t = read_tempo(s0);
    s0->reserved8C = t;

    lo = 0x03938700 / t;
    hi = 0x03938700 % t;
    if ((s32)((u32)t >> 1) < hi) {
        s0->reserved8C = lo + 1;
    } else {
        s0->reserved8C = lo;
    }

    s0->tempo94 = s0->reserved8C;
    s0->b24 = *(s0->cursor00++);
    s0->b25 = *(s0->cursor00++);

    s0->accumulator90 = s0->w84 = _SsReadDeltaValue(a0, 0);
    s0->marker10 = 0;
    m = s0->scale50 * s0->reserved8C;
    s0->start04 = s0->cursor00;
    s0->saved08 = s0->cursor00;
    s0->alternate0C = s0->cursor00;
    if (m * 10 < VBLANK_MINUS * 60) {
        s0->delay52 = (VBLANK_MINUS * 600) / m;
        s0->pitch54 = s0->delay52;
    } else {
        v0 = (s0->scale50 * s0->reserved8C * 10) / (VBLANK_MINUS * 60);
        s0->delay52 = -1;
        s0->pitch54 = v0;
        if ((VBLANK_MINUS * 30) < (s0->scale50 * s0->reserved8C * 10) % (VBLANK_MINUS * 60)) {
            s0->pitch54 = v0 + 1;
        }
    }
    s0->h56 = (u16)s0->pitch54;
    return 0;
}
