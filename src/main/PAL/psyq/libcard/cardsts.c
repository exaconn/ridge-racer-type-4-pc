/* GCC_VERSION: 970318 */
#include "types.h"

extern volatile int D_800ABDE4;
extern int D_800ABDE8;
extern unsigned char D_80114741;
extern int D_80114870;
extern s32 D_800ABDF4;
extern s32 D_800ABDF8;
extern s32 D_80114730;

int func_8009BF94();
s32 func_8009C0A4(s32 value);

int func_8009BF94(a0, a1, a2) int a0; int *a1; int *a2; {
    if (D_80114870 == 0 || D_800ABDE4 == 1) {
        *a2 = 0;
        *a1 = 0;
        return -1;
    }
    if (a0 == 0) {
        while (D_800ABDE4 < 5);
    }
    *a1 = D_80114870;
    switch (D_800ABDE4) {
    case 4:
    case 2:
        *a2 = 0;
        return 0;
    case 17:
        if (D_800ABDE8 == 1) {
            *a2 = D_800ABDE8;
        } else {
    case 33:
            *a2 = 2;
        }
        goto fin;
    default:
        if (D_80114741 == 8) {
            *a2 = 3;
        } else {
            *a2 = 0;
        }
    fin:
        D_80114870 = 0;
        return 1;
    }
}

s32 func_8009C0A4(s32 value) {
    s32 result;
    if (!(D_800ABDE4 & 1)) return 0;
    D_800ABDF4 = 0;
    D_800ABDF8 = 0;
    D_80114730 = 0;
    D_800ABDE8 = 0;
    {
        /* ASM_BARRIER: retain the original assembler temporary for the final state store. */
        register s32 *base __asm__("$1");
        result = 1;
        /* ASM_BARRIER: preserve the shared return path and preceding stores. */
        __asm__("" : "=r"(result) : "0"(result) : "$1", "memory");
        /* PAL RAM mapping: D_800ABDEC is base minus 0x4214. */
        base = (s32 *)0x800B0000U;
        /* ASM_BARRIER: expose the register-relative store in the branch delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[-0x4214 / 4] = value;
    }
    return result;
}
