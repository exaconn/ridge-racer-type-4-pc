/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/sequencer_init.h"
#include "psyq/libsnd/sstick.h"

extern void _SsSeqGetEof();

typedef struct {
    void (*noteOn)();
    void (*program)();
    void (*bend)();
    void (*meta)();
    void (*control)();
} SeqHandlers;
extern SeqHandlers SsFCALL;

s32 _SsGetSeqData(s32 a0_in, s32 a1_in) {
    s16 a0 = a0_in;
    s16 a1 = a1_in;
    u8 c;
    u8 c2;
    u8 d;
    s32 status;
    s32 ret = 0;
    LibStateB0 *p = &_ss_score[a0][a1];

    c = *p->c0++;
    if ((_ss_score[a0][a1].w98 & 0x401) == 0x401) {
        if (p->c0 == p->p10 + 1) {
            _SsSeqGetEof(a0, a1, *(p->p10 + 1));
            return -1;
        }
    }
    if (c & 0x80) {
        p->b17 = c & 0x0F;
        status = c & 0xF0;
        switch (status) {
        case 0x90: {
            u8 c1;
            p->b16 = status;
            c1 = *p->c0++;
            c2 = *p->c0++;
            p->w90 = _SsReadDeltaValue(a0, a1);
            SsFCALL.noteOn(a0, a1, c1, c2);
            break;
        }
        case 0xB0:
            p->b16 = status;
            d = *p->c0++;
            SsFCALL.control(a0, a1, d);
            break;
        case 0xC0:
            p->b16 = status;
            d = *p->c0++;
            SsFCALL.program(a0, a1, d);
            break;
        case 0xE0:
            p->b16 = status;
            p->c0++;
            SsFCALL.bend(a0, a1);
            break;
        case 0xF0:
            p->b16 = 0xFF;
            status = *p->c0++;
            status = (u8)status;
            if (status == 0x2F) {
                ret = 1;
                _SsSeqGetEof(a0, a1, 0x2F);
            } else {
                SsFCALL.meta(a0, a1, status);
            }
            break;
        }
    } else {
        switch (p->b16) {
        case 0x90:
            c2 = *p->c0++;
            p->w90 = _SsReadDeltaValue(a0, a1);
            SsFCALL.noteOn(a0, a1, c, c2);
            break;
        case 0xB0:
            SsFCALL.control(a0, a1, c);
            break;
        case 0xC0:
            SsFCALL.program(a0, a1, c);
            break;
        case 0xE0:
            SsFCALL.bend(a0, a1);
            break;
        case 0xFF:
            status = (u8)c;
            if (status == 0x2F) {
                ret = 1;
                _SsSeqGetEof(a0, a1, 0x2F);
            } else {
                SsFCALL.meta(a0, a1, status);
            }
            break;
        }
    }
    return ret;
}
