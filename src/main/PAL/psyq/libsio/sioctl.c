/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/interrupts.h"
#include "rr/serial.h"
#include "m2c_macros.h"

extern u16 D_801146CA;
extern u8 D_800ABD20[];
extern s32 func_80097714(s32,void *);
extern int func_8009A94C();
extern int func_8009A9A4();
extern int func_8009ABF8();
extern int func_8009AC50();

s32 func_8009B85C(void);
int func_8009B8EC();
s32 func_8009B9D4(void);

s32 func_8009B85C(void) {
    RrInterruptRegisters *interrupts=D_800ABD30;
    RrSerialPortRegisters *port;
    volatile u16 *control;
    interrupts->mask &= ~0x100U;
    port=D_800ABD18;
    port->control |= 0x50;
    control=&D_801146CA;
    /* The final control write occupies the original call's delay slot. */
    *(u16 *)&port->control=*control;
    func_80097714(3,D_800ABD20);
    {
        /* Share the request bases across the store/reload pairs. */
        register volatile RrSerialRequest *send=&D_801146E8;
        register RrSerialReceiveRequest *receive;
        /* ASM_BARRIER: Reload request words through the original value register. */
        register s32 active __asm__("$3");
        /* ASM_BARRIER: Keep zero in the shared return register after the sender base expires. */
        register s32 result __asm__("$2");
        /* ASM_BARRIER: Retain the request base across both store/reload pairs. */
        __asm__("" : "=r"(send) : "0"(send));
        send->active=0;
        active=send->active;
        /* ASM_BARRIER: Finish the active reload before materializing the receiver base. */
        __asm__ volatile("" : : "r"(active) : "$4", "memory");
        receive=&D_801146D8;
        /* ASM_BARRIER: Materialize the receive prefix before publishing its reloaded fields. */
        __asm__("" : "=r"(receive) : "0"(receive));
        receive->request.active=active;
        send->second=0;
        active=send->second;
        result=0;
        receive->request.second=active;
        /* ASM_BARRIER: Keep the zero result before the shared return instruction. */
        __asm__ volatile("" : : "r"(result));
        return result;
    }
}

int func_8009B8EC(a0, a1)
    int *a0;
    int a1;
{
    int v;
    a0[6] = 0;
    if (a1 == 1) {
        if (a0[0] & 0x8000) {
            if (func_8009A94C(a0[2], a0[3]) != 0) {
                a0[6] = 0x10;
                return -1;
            }
            return 0;
        }
        v = func_8009A9A4(a0[2], a0[3]);
        if (v < 0) a0[6] = 5;
        return v;
    }
    if (a1 == 2) {
        if (a0[0] & 0x8000) {
            if (func_8009ABF8(a0[2], a0[3]) == 0) return 0;
            a0[6] = 0x10;
            return -1;
        }
        v = func_8009AC50(a0[2], a0[3]);
        if (v < 0) a0[6] = 5;
        return v;
    }
    a0[6] = 0x16;
    return -1;
}

s32 func_8009B9D4(void) {
    return 0;
}
