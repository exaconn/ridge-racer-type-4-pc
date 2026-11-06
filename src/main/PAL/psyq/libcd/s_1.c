/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/cd.h"
#include "m2c_macros.h"
#include "psyq/libcd/bios_1.h"

extern u8 D_800A64A4;
extern u8 D_800A64A5;
extern M2C_UNK D_800A64A0;
extern int CD_init();
extern int CD_flush();
extern s32 D_800A6490;
extern int D_800A64AC[];
extern char D_800118C8[];
extern int D_800A652C[];
extern s32 D_800A6488;
extern s32 D_800A648C;

u8 func_8008BF94(void);
u8 func_8008BFA4(void);
u8 func_8008BFB4(void);
M2C_UNK *func_8008BFC4(void);
int CdReset();
void func_8008C03C();
s32 func_8008C05C(s32 value);
char *CdComstr(unsigned char a0);
char *CdIntstr(unsigned char a0);
s32 func_8008C0D8(s32 mode,u8 *status);
s32 func_8008C0F8(s32 mode, u8 *status);
s32 func_8008C118(s32 value);
s32 func_8008C12C(s32 value);

u8 func_8008BF94(void) {
    return D_800A6494.bytes.status;
}

u8 func_8008BFA4(void) {
    return D_800A64A4;
}

u8 func_8008BFB4(void) {
    return D_800A64A5;
}

M2C_UNK *func_8008BFC4(void) {
    return &D_800A64A0;
}

int CdReset(a0)
    int a0;
{
    if (a0 == 2) {
        CD_initintr();
        return 1;
    }
    if (CD_init() != 0) return 0;
    if (a0 == 1) {
        int r = CD_initvol();
        if (r != 0) return 0;
    }
    return 1;
}

void func_8008C03C() { CD_flush(); }

s32 func_8008C05C(s32 value) {
    s32 previous = D_800A6490;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: D_800A6490 is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(0x6490) / 4] = value;
    return previous;
}

char *CdComstr(unsigned char a0) { char *r; if (a0 < 0x1C) r = (char *)D_800A64AC[a0]; else r = D_800118C8; return r; }

char *CdIntstr(unsigned char a0) { char *r; if (a0 < 7) r = (char *)D_800A652C[a0]; else r = D_800118C8; return r; }

s32 func_8008C0D8(s32 mode,u8 *status) {
    return CD_sync(mode,status);
}

s32 func_8008C0F8(s32 mode, u8 *status) {
    return CD_ready(mode, status);
}

s32 func_8008C118(s32 value) {
    s32 previous = D_800A6488;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: D_800A6488 is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(0x6488) / 4] = value;
    return previous;
}

s32 func_8008C12C(s32 value) {
    s32 previous = D_800A648C;
    /* ASM_BARRIER: use the original assembler temporary for the store address. */
    register s32 *base __asm__("$1");
    /* ASM_BARRIER: load the prior value before materializing the store address. */
    __asm__ volatile("" : "=r"(previous) : "0"(previous) : "$1");
    /* PAL RAM mapping: D_800A648C is base plus the signed offset below. */
    base = (s32 *)0x800A0000U;
    /* ASM_BARRIER: expose a register-relative store so it can occupy the return slot. */
    __asm__("" : "=r"(base) : "0"(base));
    base[(0x648C) / 4] = value;
    return previous;
}
