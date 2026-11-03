/* GCC_VERSION: 970318 */
#include "rr/commands.h"

extern volatile int D_800ABDE8;
extern s32 func_8009D7C4(s32);

int func_8009D65C();
s32 func_8009D680(s32 value);
s32 func_8009D700(s32 request, u8 *buffer);
int func_8009D7A0();

int func_8009D65C() {
 register int r asm("$2") = 0x58; int d; d = D_800ABDE8; if (d != 2) r = 0; return r;
}

s32 func_8009D680(s32 value) {
    s32 result;
    switch(D_800ABDF8) {
    case 3: D_80114730=value & 255; break;
    case 4: *(u32 *)D_8011471C=value & 255; break;
    case 5: *D_80114720=value & 255; break;
    }
    result=0;
    return result;
}

s32 func_8009D700(s32 request, u8 *buffer) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 12;
    D_80114734 = func_8009D7A0;
    D_80114738 = func_8009D7C4;
    active = 2;
    D_80114718 = buffer;
    {
        /* ASM_BARRIER: preserve the original assembler temporary for the mapped store. */
        register s32 *base __asm__("$1");
        /* ASM_BARRIER: finish parameter publication before materializing the state address. */
        __asm__ volatile("" : : : "$1", "memory");
        /* PAL RAM mapping: base[-0x421C / 4] is D_800ABDE4. */
        base = (s32 *)0x800B0000U;
        /* ASM_BARRIER: expose the active-state store in the branch delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[-0x421C / 4] = active;
    }
    return result;
}

int func_8009D7A0() {
 register int r asm("$2") = 0x5A; int d; d = D_800ABDE8; if (d != 2) r = 0; return r;
}
