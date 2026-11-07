/* CC1_FLAGS: -mno-split-addresses */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
#include "rr/channel_stream.h"

typedef struct {
    void (*noteOn)();
    void (*program)();
    void (*bend)();
    void (*meta)();
    void (*control)();
} SeqHandlers;
extern SeqHandlers D_8010EFC8;

s32 _SsGetSeqData(s16 a0, s16 a1) {
    s32 i = a0;
    RrLibStateB0 **b = _ss_score;
    RrLibStateB0 **q = &b[i];
    s32 o = a1 * 0xB0;
    /* ASM_BARRIER: keeps the stream byte in s2 (otherwise s2/s3 swap with p). */
    register u32 c __asm__("$18");
    RrLibStateB0 *p;
    u32 c2;
    s32 ret = 0;
    s32 status;
    u32 d;

    /* ASM_BARRIER: complete row byte-offset scaling before loading the bank pointer. */
    __asm__("" : "=r"(q) : "0"(q), "r"(o));
    p = (RrLibStateB0 *)((u8 *)*q + o);
    c = *p->c0++;
    if ((((RrLibStateB0 *)(o + (s32)*q))->w98 & 0x401) == 0x401) {
        if (p->c0 == p->p10 + 1) {
            func_800833A0(a0, a1, *(p->p10 + 1));
            return -1;
        }
    }
    if (c & 0x80) {
        p->b17 = c & 0x0F;
        status = c & 0xF0;
        switch (status) {
        case 0x90:
            p->b16 = status;
            c = *p->c0++;
            c2 = *p->c0++;
            p->w90 = _SsReadDeltaValue(a0, a1);
            D_8010EFC8.noteOn(a0, a1, c, c2);
            break;
        case 0xB0: {
            /* ASM_BARRIER: schedule argument sign-extension before loading stream pointer. */
            register s32 r4 __asm__("$4") = a0;
            /* ASM_BARRIER: split sll/sra of second argument across handler load. */
            register s32 r5 __asm__("$5") = a1 << 16;
            /* ASM_BARRIER: order argument shifts before reading p->c0. */
            __asm__("" : "=r"(p) : "0"(p), "r"(r4), "r"(r5));
            p->b16 = status;
            d = *p->c0;
            p->c0++;
            D_8010EFC8.control(r4, r5 >> 16, d);
            break;
        }
        case 0xC0: {
            /* ASM_BARRIER: schedule argument sign-extension before loading stream pointer. */
            register s32 r4 __asm__("$4") = a0;
            /* ASM_BARRIER: split sll/sra of second argument across handler load. */
            register s32 r5 __asm__("$5") = a1 << 16;
            /* ASM_BARRIER: order argument shifts before reading p->c0. */
            __asm__("" : "=r"(p) : "0"(p), "r"(r4), "r"(r5));
            p->b16 = status;
            d = *p->c0;
            p->c0++;
            D_8010EFC8.program(r4, r5 >> 16, d);
            break;
        }
        case 0xE0:
            p->b16 = status;
            p->c0++;
            D_8010EFC8.bend(a0, a1);
            break;
        case 0xF0:
            p->b16 = 0xFF;
            status = *p->c0;
            p->c0++;
            status = (u8)status;
            if (status == 0x2F) {
                goto eot;
            } else {
                goto meta;
            }
            break;
        default:
            break;
        }
    } else {
        switch (p->b16) {
        case 0x90:
            c2 = *p->c0;
            p->c0++;
            p->w90 = _SsReadDeltaValue(a0, a1);
            D_8010EFC8.noteOn(a0, a1, c, c2);
            break;
        case 0xB0:
            D_8010EFC8.control(a0, a1, c);
            break;
        case 0xC0:
            D_8010EFC8.program(a0, a1, c);
            break;
        case 0xE0:
            D_8010EFC8.bend(a0, a1);
            break;
        case 0xFF:
            status = (u8)c;
            if (status == 0x2F) {
            eot:
                ret = 1;
                func_800833A0(a0, a1, 0x2F);
            } else {
            meta: {
                /* ASM_BARRIER: schedule argument sign-extension before loading handler. */
                register s32 r4 __asm__("$4") = a0;
                /* ASM_BARRIER: split sll/sra of second argument across handler load. */
                register s32 r5 __asm__("$5") = a1 << 16;
                /* ASM_BARRIER: keep meta handler pointer in v0 across sra. */
                register void (*fn)() __asm__("$2");
                /* ASM_BARRIER: emit sll of r5 before loading meta handler. */
                __asm__("" : "=r"(r5) : "0"(r5), "r"(r4));
                fn = D_8010EFC8.meta;
                /* ASM_BARRIER: emit sra of r5 after loading meta handler. */
                __asm__("" : "=r"(r5) : "0"(r5), "r"(fn));
                r5 >>= 16;
                /* ASM_BARRIER: prevent sra from filling jalr delay slot. */
                __asm__("" : "=r"(r5) : "0"(r5));
                fn(r4, r5, status);
            }
            }
            break;
        default:
            break;
        }
    }
    return ret;
}
