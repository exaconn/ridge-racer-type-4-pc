/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"

extern u32 *D_800ABBC8;
extern int *D_800ABBC4;

void func_80094AB0(u32 command);
s32 func_80094AD4(s32 index);
int func_80094AE8(int *a, int n);

void func_80094AB0(u32 command) {
    /* ASM_BARRIER: reuse v0 for the port pointer and then the command index. */
    register u32 index __asm__("$2");
    /* ASM_BARRIER: retain the original assembler temporary for the history address. */
    register u8 *base __asm__("$1");
    {
        /* ASM_BARRIER: keep the command-port pointer in v0 until the word is written. */
        register u32 *port __asm__("$2") = D_800ABBC8;
        *port = command;
    }
    index = command >> 24;
    /* ASM_BARRIER: finish the port write before binding the history address. */
    __asm__ volatile("" : "=r"(index) : "0"(index) : "$1", "memory");
    /* PAL RAM mapping: D_801127D0 is base plus 0x27D0. */
    base = (u8 *)0x80110000U;
    /* ASM_BARRIER: retain the base-plus-index byte store in the return delay slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base += index;
    base[0x27D0] = command;
}

s32 func_80094AD4(s32 index) {
    /* PAL RAM mapping: D_801127D0 is 0x27D0 bytes into this page. */
    u8 *base = (u8 *)0x80110000U;
    /* ASM_BARRIER: retain the base register through the indexed byte load. */
    __asm__("" : "=r"(base) : "0"(base));
    base += index;
    return base[0x27D0];
}

int func_80094AE8(int *a, int n) {
    int i = n - 1;
    *D_800ABBC8 = 0x04000000;
    if (n) {
        do { int v = *a; a++; i--; *D_800ABBC4 = v; } while (i != -1);
    }
    return 0;
}
