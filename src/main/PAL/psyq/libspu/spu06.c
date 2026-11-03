/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"

extern unsigned short *D_800A47D8;
extern int D_800A4800;
extern unsigned int *D_800A47E8;
extern unsigned int *D_800A47EC;

int func_8007F4B8(int a0, int a1);
void func_8007F4F4(int a0);
void func_8007F54C(void);
void func_8007F574(void);
void func_8007F59C(void);

int func_8007F4B8(int a0, int a1) {
    unsigned short v = D_800A47D8[a0];
    if (a1 == -1) return v;
    return v << D_800A4800;
}

void func_8007F4F4(int a0) {
    *D_800A47E8 &= 0xFFF8FFFF;
    if (a0) {
        *D_800A47E8 |= 0x30000;
    } else {
        *D_800A47E8 |= 0x50000;
    }
}

void func_8007F54C(void) {
    *D_800A47EC = (*D_800A47EC & 0xF0FFFFFF) | 0x20000000;
}

void func_8007F574(void) {
    *D_800A47EC = (*D_800A47EC & 0xF0FFFFFF) | 0x22000000;
}

void func_8007F59C(void) {
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
