/* GCC_VERSION: 2.7.2 */

#include "rr/gpu.h"

#include "rr/timing.h"

#include "rr/interrupts.h"

extern s32 func_8001E84C(const char *format,...);

extern char D_80011F98[];

s32 get_alarm(void) {
    s32 old,result;
    s32 saved;
    if(D_800ABBFC>=VSync(-1)) {
        s32 *counter=&D_800ABC00;
        {
            s32 tick=*counter;
            old=tick;
            tick=(u32)tick+1;
            *counter=tick;
        }
        if(old<=0xF0000) goto ok;
    }
    {
        volatile u32 *gpu=GPU_STATUS;
        const char *format;
        u32 head;
        format=D_80011F98;
        *gpu;
        /* ASM_BARRIER: Keep the discarded status read before reading queue indices. */
        __asm__("" : : : "memory");
        head=_qin;
        /* ASM_BARRIER: Read the queue head before preparing the remaining diagnostic operands. */
        __asm__("" : "=r"(head) : "0"(head));
        func_8001E84C(format,(head-(u32)_qout)&63,
            *gpu,*DMA2_CHCR,*DMA2_MADR);
    }
    saved=SetIntrMask(0);
    _qout=0;
    D_800ABBF8=saved;
    _qin=_qout;
    *DMA2_CHCR=0x401;
    *DPCR|=0x800;
    *GPU_STATUS=0x02000000;
    *GPU_STATUS=0x01000000;
    SetIntrMask(D_800ABBF8);
    result=-1;
    goto done;
ok:
    result=0;
done:
    return result;
}
