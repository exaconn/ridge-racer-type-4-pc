#include "rr/library.h"
#include "rr/channel_stream.h"
#include "psyq/libsnd/cc_6.h"
#include "rr/spu_tone_control.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_tone_update.h"

/* libsnd _SsContDataEntry (cc_6.o): SEQ control change 6, data entry.
 * Same source as bashalarmistdecomp/eternal-silver-sssc-decomp libsnd_cc_6.c. */


  /* _ss_score */

void _SsContDataEntry(s32 raw_bank, s32 raw_row, s32 raw_data) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    u8 data = raw_data;
    LibStateB0 *s;
    ProgAtr progAtr;
    VagAtr vagAtr;
    u8 ch;
    s32 i;
    s16 tune;

    s = &_ss_score[bank][row];
    ch = s->b17;
    SsUtGetProgAtr(s->b26, s->b37[ch], (u8 *)&progAtr); /* SsUtGetProgAtr */
    if (s->b1c == 1 && s->b15 == 0) {
        /* loop start armed: data is the repeat count */
        s->b1d = data;
        s->b1c = 0;
        s->b15 = 1;
        s->w90 = _SsReadDeltaValue(bank, row);
    } else if (s->b1e == 2) {
        /* RPN complete: 0 = bend range, 1 = fine tune, 2 = coarse tune */
        if (s->b19 == 0) {
            for (i = 0; i < progAtr.attributes.tones; i++) {
                SsUtGetVagAtr(s->b26, s->b37[ch], (s16)i, (u8 *)&vagAtr);
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
                SsUtSetVagAtr(s->b26, s->b37[ch], (s16)i, (u8 *)&vagAtr);
            }
        }
        s->w90 = _SsReadDeltaValue(bank, row);
        s->b1e = 0;
    } else if (s->b1f == 2) {
        /* NRPN complete: the original incoming tone scratch is uninitialized. */
        if (s->b1b == 16) {
            SsFCALLCcEntry[s->b1a](s->b26, s->b37[ch], 0, vagAtr, s->b1a, data);
        } else {
            SsFCALLCcEntry[s->b1a](s->b26, s->b37[ch], s->b1b, vagAtr, s->b1a, data);
        }
        s->w90 = _SsReadDeltaValue(bank, row);
        s->b1f = 0;
    } else {
        s->w90 = _SsReadDeltaValue(bank, row);
    }
}
