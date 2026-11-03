/* GCC_VERSION: 970318 */
#include "rr_asm.h"
#include "rr/commands.h"
#include "rr/serial.h"
#include "psyq/libcard/cardc11.h"

extern s32 func_8009CF94(s32);

s32 func_8009CCF0(s32 value);
s32 func_8009CDFC(s32 request, s32 parameter, u8 *buffer, u8 *first);

s32 func_8009CCF0(s32 value) {
    s32 saved=value;
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
        {
            s32 count=D_80114728;
            s32 phase=D_800ABDF8;
            if((s32)((u32)count+5)<phase) {
                /* Preserve native pointer width and the original wrapping 32-bit difference. */
                unsigned long address=(s32)((u32)phase-(u32)count);
                address+=(unsigned long)D_80114718;
                ((u8 *)address)[-6]=saved;
            }
        }
    }
    result=0;
    return result;
}

s32 func_8009CDFC(s32 request, s32 parameter, u8 *buffer, u8 *first) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 15;
    D_80114734 = func_8009CEBC;
    D_80114738 = func_8009CF94;
    active = 2;
    D_80114724 = parameter;
    D_80114718 = buffer;
    D_8011471C = first;
    D_80114728 = 0;
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
