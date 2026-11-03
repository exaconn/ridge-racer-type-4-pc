/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"

extern int D_800A67CC[];
extern int D_800A67D4[];
extern int func_8008B154(), func_8008EB6C(), func_8008C0F8();
extern s32 D_800A63F0;
extern s32 D_800A63F4;

int func_8008F090();
s32 func_8008F194(s32 value);
s32 func_8008F1A8(s32 value);

int func_8008F090(a0, a1) int a0, a1; {
    int s0;
    int *p = D_800A67CC;
    int *q = D_800A67CC + 2;
    for (;;) {
        if (func_8008B154(-1) > p[0] + 0x4B0) {
            s0 = -1;
        } else if (p[-2] < 0 || func_8008B154(-1) > p[-1] + 0x3C) {
            func_8008EB6C(1);
            s0 = p[-7];
        } else {
            s0 = p[-2];
        }
        if (a0 == 0) {
            if (q[0] == 0 || s0 != 0) { if (s0 > 0) continue; break; }
            continue;
        }
        break;
    }
    func_8008C0F8(1, a1);
    if (q[0] != 0 && s0 == 0) s0 = 1;
    return s0;
}

s32 func_8008F194(s32 value) {
    s32 previous = D_800A63F0;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: D_800A63F0 is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(0x63F0) / 4] = value;
    return previous;
}

s32 func_8008F1A8(s32 value) {
    s32 previous = D_800A63F4;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: D_800A63F4 is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(0x63F4) / 4] = value;
    return previous;
}
