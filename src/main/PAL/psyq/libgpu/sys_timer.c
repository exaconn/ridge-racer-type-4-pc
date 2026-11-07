/* GCC_VERSION: 2.7.2 */
#include "rr/gpu.h"
#include "rr/timing.h"
#include "rr/interrupts.h"

extern s32 func_8001E84C(const char *format,...);
extern char D_80011F98[];

void set_alarm(void) {
    D_800ABBFC = (u32)VSync(-1) + 0xF0;
    D_800ABC00 = 0;
}

int get_alarm(void) {
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
        volatile u32 *gpu=D_800ABBC8;
        const char *format;
        u32 head;
        format=D_80011F98;
        *gpu;
        /* ASM_BARRIER: Keep the discarded status read before reading queue indices. */
        __asm__("" : : : "memory");
        head=D_800ABBE8;
        /* ASM_BARRIER: Read the queue head before preparing the remaining diagnostic operands. */
        __asm__("" : "=r"(head) : "0"(head));
        func_8001E84C(format,(head-(u32)D_800ABBEC)&63,
            *gpu,*D_800ABBD4,*D_800ABBCC);
    }
    saved=SetIntrMask(0);
    D_800ABBEC=0;
    D_800ABBF8=saved;
    D_800ABBE8=D_800ABBEC;
    *D_800ABBD4=0x401;
    *D_800ABBE4|=0x800;
    *D_800ABBC8=0x02000000;
    *D_800ABBC8=0x01000000;
    SetIntrMask(D_800ABBF8);
    result=-1;
    goto done;
ok:
    result=0;
done:
    return result;
}

s32 func_800954D8(s32 a) {
    *D_800ABBC8 = 0x10000007;
    if ((*D_800ABBC4 & 0xFFFFFF) != 2) {
        *D_800ABBC4 = (*D_800ABBC8 & 0x3FFF) | 0xE1001000;
        *D_800ABBC4;
        return 0;
    }
    if (!(a & 8)) return 1;
    /* Ordinary final publication exposes the original branch-delay store. */
    *(u32 *)D_800ABBC8 = 0x09000001;
    return 2;
}
