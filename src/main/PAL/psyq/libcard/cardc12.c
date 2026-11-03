/* GCC_VERSION: 970318 */
#include "rr_asm.h"
#include "rr/commands.h"
#include "rr/serial.h"

extern s32 func_8009C610(s32);

s32 func_8009CF94(s32 unused);
s32 func_8009D068(s32 request, s32 parameter, s32 count);
s32 func_8009D118(void);
s32 func_8009D198(s32 request, s32 parameter);
s32 func_8009D238(s32 request, s32 parameter, s32 count);
int func_8009D2E0();
s32 func_8009D348(s32 request, s32 parameter);
int func_8009D3E8();

s32 func_8009CF94(s32 unused) {
    s32 received;
    RrSerialPortRegisters *port;
    s32 result;
    if(D_800ABDF8==3) {
        port=D_800ABDE0;
        RR_NOP_DEP_ON(port);
        while(!(port->status & 2)) { }
        D_80114728=D_800ABDE0->data;
    } else if(D_800ABDF8>=4) {
        if((u32)D_800ABDF8==(u32)D_80114728+4) {
            port=D_800ABDE0;
            RR_NOP_DEP_ON(port);
            while(!(port->status & 2)) { }
            received=D_800ABDE0->data;
            D_80114718=D_8011471C;
            D_8011472C.value=1;
            D_80114730=received+1;
        }
    }
    result=0;
    return result;
}

s32 func_8009D068(s32 request, s32 parameter, s32 count) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 6;
    D_80114734 = func_8009D118;
    D_80114738 = func_8009C610;
    active = 2;
    D_80114724 = parameter;
    D_80114728 = count;
    D_8011472C.value = 0;
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

s32 func_8009D118(void) {
    s32 result;
    switch(D_800ABDE8) {
    case 2: result=0x5D; break;
    case 4: result=D_80114724; break;
    case 5: result=D_8011472C.value; break;
    case 6: result=D_80114728; break;
    default: result=0; break;
    }
    return result;
}

s32 func_8009D198(s32 request, s32 parameter) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 7;
    D_80114734 = func_8009D118;
    D_80114738 = func_8009C610;
    active = 2;
    D_8011472C.value = parameter;
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

s32 func_8009D238(s32 request, s32 parameter, s32 count) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 13;
    D_80114734 = func_8009D2E0;
    D_80114738 = func_8009C610;
    active = 2;
    D_80114724 = parameter;
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

int func_8009D2E0() {
    int v = D_800ABDE8;
    int r;
    if (v == 4) goto c4;
    if (v < 5) {
        if (v == 2) goto c2;
        r = 0;
        goto end;
    }
    if (v == 5) goto c5;
    r = 0;
    goto end;
c2:
    r = 0x5E;
    goto end;
c4:
    r = D_80114724;
    goto end;
c5:
    r = D_80114728;
end:
    return r;
}

s32 func_8009D348(s32 request, s32 parameter) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 8;
    D_80114734 = func_8009D3E8;
    D_80114738 = func_8009C610;
    active = 2;
    D_80114724 = parameter;
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

int func_8009D3E8() {
    switch (D_800ABDE8) {
    case 2: return 0x5F;
    case 4: return D_80114724;
    }
    return 0;
}
