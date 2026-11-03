/* RODATA 0x80012158 0x20 */
/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/commands.h"

extern s32 func_8009DF14(s32, s32);
extern unsigned char D_80114725;
extern unsigned char D_80114726;
extern unsigned char D_80114727;
extern s32 func_8009CA8C(void);
extern s32 func_8009CB50(s32);

s32 func_8009C638(s32 request, u8 *buffer);
int func_8009C6D8();
s32 func_8009C6F8(s32 value);
s32 func_8009C768(s32 request, u8 *buffer, s32 sector, s32 input_count);
int func_8009C87C();
s32 func_8009C91C(s32 value);
s32 func_8009C978(s32 request, u8 *buffer, s32 sector, s32 input_count);

s32 func_8009C638(s32 request, u8 *buffer) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 3;
    D_80114734 = func_8009C6D8;
    D_80114738 = func_8009C6F8;
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

int func_8009C6D8() {
    register int r asm("$2");
    register int k asm("$2");
    int d = D_800ABDE8;
    k = 2;
    if (d == k) r = 0x5B;
    else r = 0;
    return r;
}

s32 func_8009C6F8(s32 value) {
    s32 phase=D_800ABDF8;
    /* ASM_BARRIER: Retain the copied input for status-byte publication. */
    register s32 saved __asm__("$5")=value;
    /* ASM_BARRIER: Share the zero result after all response stores. */
    register s32 result __asm__("$2");
    if(phase==5) {
        D_80114730=saved & 255;
        /* ASM_BARRIER: Keep the copied raw input distinct from its masked status. */
        __asm__ volatile("" : : "r"(saved));
    }
    else if(phase==10) D_80114718[4]=(u32)value-1;
    else if(phase>=6) D_80114718[phase-6]=value;
    result=0;
    /* ASM_BARRIER: Preserve the common return after response publication. */
    __asm__ volatile("" : : "r"(result));
    return result;
}

s32 func_8009C768(s32 request, u8 *buffer, s32 sector, s32 input_count) {
    s32 result;
    s32 active;
    /* ASM_BARRIER: retain the count in the original callee-saved register across validation calls. */
    register s32 count __asm__("$17") = input_count;
    /* ASM_BARRIER: finish preserving count before preparing the first validation call. */
    __asm__("" : "=r"(count) : "0"(count));
    if (!func_8009DF14(0, sector)) return 0;
    if (!func_8009DF14(0, (s32)((u32)sector + (u32)count - 1))) return 0;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 4;
    D_80114734 = func_8009C87C;
    D_80114738 = func_8009C91C;
    active = 2;
    D_80114718 = buffer;
    D_80114724 = sector;
    D_80114728 = count;
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

/* legacy typing of shared globals in this part of the unit */
#define D_80114724 (*(unsigned char (*))&D_80114724)

int func_8009C87C() {
    switch (D_800ABDE8) {
    case 2: return 0x5B;
    case 3: return 1;
    case 5: return D_80114724;
    case 6: return D_80114725;
    case 7: return D_80114726;
    case 8: return D_80114727;
    case 9: return D_80114728;
    }
    return 0;
}

#undef D_80114724

s32 func_8009C91C(s32 value) {
    s32 phase=D_800ABDF8;
    s32 result;
    if(phase==9) D_80114730=(u32)D_80114728+1;
    else if(phase>=11) D_80114718[phase-11]=value;
    result=0;
    return result;
}

s32 func_8009C978(s32 request, u8 *buffer, s32 sector, s32 input_count) {
    s32 result;
    s32 active;
    /* ASM_BARRIER: retain the count in the original callee-saved register across validation calls. */
    register s32 count __asm__("$17") = input_count;
    /* ASM_BARRIER: finish preserving count before preparing the first validation call. */
    __asm__("" : "=r"(count) : "0"(count));
    if (!func_8009DF14(1, sector)) return 0;
    if (!func_8009DF14(1, (s32)((u32)sector + (u32)count - 1))) return 0;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 5;
    D_80114734 = func_8009CA8C;
    D_80114738 = func_8009CB50;
    active = 2;
    D_80114718 = buffer;
    D_80114724 = sector;
    D_80114728 = count;
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
