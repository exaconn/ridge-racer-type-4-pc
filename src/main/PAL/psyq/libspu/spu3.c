/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "psyq/libspu/spu3.h"
#include "rr/spu.h"

extern unsigned int *SPU_DELAY;

void _spu_FsetDelayW(void);

void _spu_FsetDelayW(void) {
    *SPU_DELAY = (*SPU_DELAY & 0xF0FFFFFF) | 0x20000000;
}

void _spu_FsetDelayR(void) {
    *SPU_DELAY = (*SPU_DELAY & 0xF0FFFFFF) | 0x22000000;
}

void _spu_Fw1ts(void) {
    /* Unsigned multiplication reproduces the original wrapping delay arithmetic.
     * The counter's views keep each loop load/store; its initial normal
     * store can occupy the entry jump's delay slot. */
    struct { s32 counter; volatile u32 accumulator; } work;
    work.accumulator=13;
    work.counter=0;
    goto test;
body:
    {
        /* ASM_BARRIER: Keep the loaded accumulator separate from the multiply result. */
        register u32 value __asm__("$3") = work.accumulator;
        work.accumulator=value*13;
    }
    *(s32 *)&work.counter=*(volatile s32 *)&work.counter+1;
test:
    if(*(s32 *)&work.counter<60) goto body;
}
