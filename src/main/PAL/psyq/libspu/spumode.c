/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "psyq/libspu/spuwait.h"
#include "psyq/libspu/spuregs2.h"
#include "rr/spu.h"

short SsVabTransCompleted(short a0);

void SpuSetTransferMode(s32 mode) {
    s32 selected;
    /* ASM_BARRIER: preserve the original assembler temporary for the mapped store. */
    register s32 *base __asm__("$1");
    switch (mode) {
    case 0: selected = 0; break;
    case 1: selected = 1; break;
    default: selected = 0; break;
    }
    _spu_trans_mode = mode;
    /* ASM_BARRIER: finish the preceding state updates before binding the address. */
    __asm__ volatile("" : : "r"(selected) : "$1");
    /* PAL RAM mapping for _spu_transMode. */
    base = (s32 *)0x800A0000U;
    /* ASM_BARRIER: allow the register-relative store in the branch delay slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[0x47F4 / 4] = selected;
}

short SsVabTransCompleted(short a0) { return SpuIsTransferCompleted(a0); }
