#include "types.h"
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
