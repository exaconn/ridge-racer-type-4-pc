/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/gpu.h"
#include "rr/timing.h"
#include "rr/interrupts.h"

extern s32 func_8001E84C(const char *format,...);
extern char D_80011F98[];

int func_80095224();
void func_80095360(void);
s32 func_80095394(void);
s32 func_800954D8(s32 a);

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBEC (*(int (*))&D_800ABBEC)
#define D_800ABBD4 (*(volatile int *(*))&D_800ABBD4)
#define D_800ABBC8 (*(volatile int *(*))&D_800ABBC8)

int func_80095224(a0) int a0; {
    int s;
    int v;
    if (a0 == 0) {
        func_80095360();
        goto L5C;
L44:
        func_80094E74();
        if (func_80095394() != 0) return -1;
L5C:
        if (D_800ABBE8 != D_800ABBEC) goto L44;
        goto L90;
L80:
        if (func_80095394() != 0) return -1;
L90:
        if (*D_800ABBD4 & 0x01000000) goto L80;
        if (!(*D_800ABBC8 & 0x04000000)) goto L80;
        return 0;
    }
    s = (D_800ABBE8 - D_800ABBEC) & 0x3F;
    if (s != 0) func_80094E74();
    if (*D_800ABBD4 & 0x01000000) goto L344;
    if (*D_800ABBC8 & 0x04000000) goto retS;
L344:
    if (s != 0) return s;
    return 1;
retS:
    return s;
}

#undef D_800ABBEC
#undef D_800ABBD4
#undef D_800ABBC8

void func_80095360(void) {
    D_800ABBFC = (u32)func_8008B154(-1) + 0xF0;
    D_800ABC00 = 0;
}

s32 func_80095394(void) {
    s32 old,result;
    s32 saved;
    if(D_800ABBFC>=func_8008B154(-1)) {
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
    saved=func_8008B4E0(0);
    D_800ABBEC=0;
    D_800ABBF8=saved;
    D_800ABBE8=D_800ABBEC;
    *D_800ABBD4=0x401;
    *D_800ABBE4|=0x800;
    *D_800ABBC8=0x02000000;
    *D_800ABBC8=0x01000000;
    func_8008B4E0(D_800ABBF8);
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
