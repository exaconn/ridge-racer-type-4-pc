#include "types.h"
#include "rr/counters.h"
#include "rr/spu_tick_control.h"

extern int *D_800ABC30;
extern int RCntIntrMaskBits[];

int SetRCnt(unsigned a0, unsigned short a1, int a2);
int StartRCnt();
s32 StopRCnt(u32 index);

int SetRCnt(unsigned a0, unsigned short a1, int a2) {
    int t = a0 & 0xFFFF;
    int a3 = 0x48;
    char *p;
    if (t >= 3) return 0;
    p = (char *)(t * 16 + (unsigned long)RCNT_REGS);
    ((short *)p)[2] = 0;
    ((volatile short *)p)[4] = a1;
    if (t < 2U) {
        if (a2 & 0x10) a3 = 0x49;
        if (!(a2 & 1)) a3 |= 0x100;
    } else {
        if (t == 2) {
            if (!(a2 & 1)) a3 = 0x248;
        }
    }
    if (a2 & 0x1000) a3 |= 0x10;
    ((short *)(t * 16 + (unsigned long)RCNT_REGS))[2] = a3;
    return 1;
}

s32 GetRCnt(u32 id) {
    s32 index=id & 0xFFFF;
    if(index>=3) return 0;
    return RCNT_REGS[index].count;
}

int StartRCnt(a0) unsigned int a0; {
  int v = a0 & 0xFFFF;
  int *p = D_800ABC30;
  p[1] |= RCntIntrMaskBits[v];
  return v < 3;
}

s32 StopRCnt(u32 index) {
    int v = index & 0xFFFF;
    int *p = D_800ABC30;
    p[1] &= ~((int *)((u8 *)0x800B0000U - 0x43C8))[v];
    return 1;
}

int ResetRCnt(a0) unsigned int a0; {
  int v = a0 & 0xFFFF;
  if (v >= 3) return 0;
  *(short *)((u8 *)RCNT_REGS + v * 16) = 0; return 1;
}
