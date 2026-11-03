/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"

#define R(o) (*(unsigned short *)(D_800A6760 + (o)))

extern unsigned char *D_800A674C, *D_800A6750, *D_800A6754, *D_800A6758;
extern char *D_800A6760;
extern s32 D_800A6498;
extern void func_8008DDAC(void);

int func_8008D730(void);
void func_8008D820(void);

int func_8008D730(void) {
    unsigned char b[4];
    if (R(0x1B8) == 0 && R(0x1BA) == 0) {
        R(0x180) = 0x3FFF;
        R(0x182) = 0x3FFF;
    }
    R(0x1B0) = 0x3FFF;
    R(0x1B2) = 0x3FFF;
    R(0x1AA) = 0xC001;
    b[2] = 0x80;
    b[0] = 0x80;
    b[3] = 0;
    b[1] = 0;
    *D_800A674C = 2;
    *D_800A6754 = b[0];
    *D_800A6758 = b[1];
    *D_800A674C = 3;
    *D_800A6750 = b[2];
    *D_800A6754 = b[3];
    *D_800A6758 = 0x20;
    return 0;
}

void func_8008D820(void) {
    D_800A648C = 0;
    D_800A6488 = 0;
    D_800A6498 = 0;
    {
        /* ASM_BARRIER: retain the original assembler temporary for the final state store. */
        register s32 *base __asm__("$1");
        /* ASM_BARRIER: complete preceding clears before materializing the last state address. */
        __asm__ volatile("" : : : "$1", "memory");
        /* PAL mapping: base[0x6494 / 4] is D_800A6494. */
        base = (s32 *)0x800A0000U;
        /* ASM_BARRIER: expose the last clear in the driver-reset call's delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[0x6494 / 4] = 0;
    }
    func_8008B364();
    func_8008B394(2, func_8008DDAC);
}
