/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

#define P ((LibStateB0 *)(offset + (unsigned long)*q))

extern LibStateB0 *_ss_score[];

extern void _SsVmSetSeqVol(s32,s32,s32,s32), _SsVmSeqKeyOff(s32);

extern u32 _snd_openflag;

extern short _snd_seq_t_max;

void _SsClose(s32 raw_bank) {
    /* The bank selects one of the 32 entries in _ss_score. */
    s16 a0=raw_bank;
    s32 i;
    s32 offset;
    /* ASM_BARRIER: Retain the original register for argument, loop, or store scheduling. */
    register s32 argument_bank __asm__("$4");
    register s32 zero;
    s32 minus_one;
    register s32 maximum;
    LibStateB0 **q;
    argument_bank=a0;
    /* ASM_BARRIER: Prepare the bank argument before reusing a1 for zero. */
    __asm__("" : "+r"(argument_bank) : : "$5");
    zero=0;
    /* ASM_BARRIER: Keep the two zero arguments independent in their original registers. */
    __asm__("" : "+r"(zero));
    _SsVmSetSeqVol(argument_bank, zero, 0, 1);
    _SsVmSeqKeyOff(a0);
    _snd_openflag &= ~(1U << a0);
    i = 0;
    if (_snd_seq_t_max > 0) {
        unsigned long b; b = (unsigned long)_ss_score; q = (LibStateB0 **)(a0 * sizeof(*q) + b);
        minus_one=-1;
        maximum=127;
        /* ASM_BARRIER: Keep the loop count distinct from the zero byte offset. */
        __asm__("" : "+r"(i) : "r"(q), "r"(minus_one), "r"(maximum));
        offset=0;
        do {
            P->w98 = 0;
            P->b22[0] = minus_one;
            P->b22[1] = 0;
            P->h48[0] = 0;
            P->h48[1] = 0;
            P->w9c = 0;
            P->wa0 = 0;
            P->h48[2] = 0;
            P->wac = 0;
            P->wa8 = 0;
            P->wa4 = 0;
            P->h48[3] = 0;
            P->h58[0] = maximum;
            i++;
            {
                register LibStateB0 *last=P;
                offset+=sizeof(LibStateB0);
                last->h58[1]=maximum;
            }
        } while (i < _snd_seq_t_max);
    }
}
