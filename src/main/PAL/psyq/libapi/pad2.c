/* GCC_VERSION: 2.7.2 */
#include "rr/interrupts.h"
#include "psyq/libapi/pad1.h"
#include "psyq/libapi/pad2.h"
#include "rr/spu_tick_control.h"

extern int func_80097714();
extern volatile short *D_800ABC54;

int RemovePatchPad();

int RemovePatchPad() { func_80096F04(); func_80097714(1, &D_801143E8); func_80096F14(); return 1; }

int func_80097618(void) {
    volatile int i; int pad[2];
    D_800ABC54[5] = 0;
    for (i = 10; --i != -1; ) ;
    return 0;
}

s32 func_80097680(void) {
    RrInterruptRegisters *registers=D_800ABC58;
    /* ASM_BARRIER: Keep the predicate result in the original return register. */
    register s32 result;
    if(registers->mask&1) {
        if(registers->pending&1) result=1;
        else {
            result=0;
            /* ASM_BARRIER: Preserve the conditional false-result block instead of folding the predicate. */
            __asm__ volatile("" : : "r"(result));
        }
    } else result=0;
    return result;
}
