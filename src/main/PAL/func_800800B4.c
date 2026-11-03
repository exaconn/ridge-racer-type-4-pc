/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"

/* libsnd _SsContDataEntry (cc_6.o): SEQ control change 6, data entry.
 * Same source as bashalarmistdecomp/eternal-silver-sssc-decomp libsnd_cc_6.c. */
typedef struct {
    u8 prior, mode, vol, pan, center, shift, min, max;
    u8 vibW, vibT, porW, porT, pbmin, pbmax, reserved1, reserved2;
    u16 adsr1, adsr2;
    s16 prog, vag;
    s16 reserved[4];
} VagAtr;
typedef struct {
    u8 tones, mvol, prior, mode, mpan, reserved0;
    s16 attr;
    u32 reserved1, reserved2;
} ProgAtr;
RR_ASSERT_SIZE(VagAtr, 0x20);
RR_ASSERT_SIZE(ProgAtr, 0x10);

extern RrLibStateB0 *D_8010F068[];  /* _ss_score */
extern void (*D_8010F00C[])();      /* SsFCALL.ccentry: _SsSetNrpnVabAttr0.. */
extern void func_800850C4(s16 vab, s16 prog, s16 tone, VagAtr *atr); /* SsUtGetVagAtr */
extern void func_800867E4(s16 vab, s16 prog, s16 tone, VagAtr *atr); /* SsUtSetVagAtr */

void func_800800B4(s16 bank, s16 row, u8 data) {
    RrLibStateB0 *s;
    ProgAtr progAtr;
    VagAtr vagAtr;
    u8 ch;
    s32 i;
    s16 tune;

    s = &D_8010F068[bank][row];
    ch = s->b17;
    func_80084FB4(s->b26, s->b37[ch], (u8 *)&progAtr); /* SsUtGetProgAtr */
    if (s->b1c == 1 && s->b15 == 0) {
        /* loop start armed: data is the repeat count */
        s->b1d = data;
        s->b1c = 0;
        s->b15 = 1;
        s->w90 = func_80081E04(bank, row);
    } else if (s->b1e == 2) {
        /* RPN complete: 0 = bend range, 1 = fine tune, 2 = coarse tune */
        if (s->b19 == 0) {
            for (i = 0; i < progAtr.tones; i++) {
                func_800850C4(s->b26, s->b37[ch], i, &vagAtr);
                switch (s->b18) {
                case 0:
                    vagAtr.pbmin = vagAtr.pbmax = data & 0x7F;
                    break;
                case 1:
                    /* Dead (the if/else below always sets tune); the binary keeps its test. */
                    if (data < 0x40) {
                        tune = (data * 100 / 8192) * 8192;
                    }
                    if (data > 0x40 && data < 0x80) {
                        tune = (data * 100 / 8192) * 8192;
                    } else {
                        tune = 0;
                    }
                    /* tune's low byte is always 0, so this u8 add changes nothing. */
                    vagAtr.shift += tune;
                    break;
                case 2:
                    if (data < 0x40) {
                        tune = data * 6400;
                    }
                    if (data >= 0x40 && data < 0x80) {
                        tune = data * 6400;
                    } else {
                        tune = 0;
                    }
                    vagAtr.center += tune;
                    break;
                }
                func_800867E4(s->b26, s->b37[ch], i, &vagAtr);
            }
        }
        s->w90 = func_80081E04(bank, row);
        s->b1e = 0;
    } else if (s->b1f == 2) {
        /* NRPN complete: tone attribute handler; vagAtr is whatever the stack holds. */
        if (s->b1b == 16) {
            D_8010F00C[s->b1a](s->b26, s->b37[ch], 0, vagAtr, s->b1a, data);
        } else {
            D_8010F00C[s->b1a](s->b26, s->b37[ch], s->b1b, vagAtr, s->b1a, data);
        }
        s->w90 = func_80081E04(bank, row);
        s->b1f = 0;
    } else {
        s->w90 = func_80081E04(bank, row);
    }
}
