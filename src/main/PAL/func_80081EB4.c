/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr/layout.h"

typedef struct {
    u8 *c0;
    u8 *c4;
    u8 *c8;
    u8 *cC;
    u8 *p10;
    u8 b14, b15, b16;
    u8 b17, b18, b19, b1a, b1b, b1c, b1d, b1e, b1f;
    s8 b20;
    u8 b21;
    s8 b22[2];
    u8 b24, b25;
    s8 b26;
    u8 b27[0x10];
    u8 b37[0x10];
    char p47[1];
    s16 h48[4];
    s16 h50;
    s16 h52;
    u16 h54;
    u16 h56;
    u16 h58[4];
    u16 h60[0x10];
    s16 h80;
    char p82[2];
    s32 w84;
    s32 w88;
    s32 w8c;
    s32 w90;
    s32 w94;
    s32 w98;
    s32 w9c;
    s32 wa0, wa4, wa8, wac;
} SeqStruct;

extern SeqStruct *D_8010F068[];
extern u32 D_8010F064;
extern char D_80011718[];
extern char D_80011730[];
extern s32 func_8001E84C(char *);
extern s32 func_80081E04(s32, s32);

/* Read the 24-bit big-endian tempo (microseconds per quarter note).
 * The read position is re-read from memory here (volatile access), as
 * the original reloads it instead of reusing the cached pointer. */
static inline s32 read_tempo(SeqStruct *c) {
    u8 *p;
    s32 b0, b1, b2;

    p = *(u8 *volatile *)&c->c0;
    b0 = *p++;
    c->c0 = p;
    b1 = *p;
    c->c0 = p + 1;
    b2 = p[1];
    c->c0 = p + 2;
    return b2 | ((b0 << 16) | (b1 << 8));
}

/* Initialise sequence slot a0 (vab id a1) from SEQ data at seq. */
s32 func_80081EB4(s16 a0, s32 a1, u8 *seq) {
    s32 i;
    u8 c;
    s32 b0, b1;
    s32 lo, hi, t;
    u32 m, v0;
    u8 *q;
    SeqStruct *s0 = *(D_8010F068 + a0);

    s0->b26 = a1;
    s0->h50 = 0;
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
    s0->h56 = 0;
    s0->b21 = 0;
    s0->b20 = 1;
    s0->b14 = 0;
    s0->w90 = 0;
    s0->b1c = 0;
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

    s0->h52 = 1;
    s0->c0 = seq;
    c = *s0->c0;
    if (c == 'S' || c == 'p') {
        s0->c0 += 7;
        if (*(s0->c0++) != 1) {
            func_8001E84C(D_80011718);
            return -1;
        }
    } else {
        func_8001E84C(D_80011730);
        return 0;
    }

    b0 = *(s0->c0++);
    b1 = *(s0->c0++);
    s0->h50 = b1 | (b0 << 8);

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
    s0->b24 = *(s0->c0++);
    s0->b25 = *(s0->c0++);

    s0->w90 = s0->w84 = func_80081E04(a0, 0);
    s0->p10 = 0;
    m = s0->h50 * s0->w8c;
    /* Loop/section pointers: the original reloads c0 for each copy. */
    q = *(u8 *volatile *)&s0->c0;
    s0->c4 = *(u8 *volatile *)&s0->c0;
    s0->cC = q;
    s0->c8 = s0->c0;
    if (m * 10 < D_8010F064 * 60) {
        s0->h52 = (D_8010F064 * 600) / m;
        s0->h54 = s0->h52;
    } else {
        v0 = (u32)(s0->h50 * s0->w8c * 10) / (D_8010F064 * 60);
        s0->h52 = -1;
        s0->h54 = v0;
        if ((D_8010F064 * 30) < (u32)(s0->h50 * s0->w8c * 10) % (D_8010F064 * 60)) {
            s0->h54 = v0 + 1;
        }
    }
    s0->h56 = s0->h54;
    return 0;
}
