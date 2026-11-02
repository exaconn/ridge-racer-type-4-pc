#include "rr/overlay666_pulse.h"
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((RrGpuScratchPointers *)0x1F800000)
#endif
void ovl666_func_801152B0(s32 x,s32 y)
{
    RrGpuScratchPointers *scratch=RR_GPU_SCRATCH;
    u8 *cursor=scratch->cursor;
    s32 wave=func_80090424(((u32)D_800ABEB0<<5)&0xFE0)/32;
    u16 mode=57;
    if (wave<0) {mode=89;wave=-wave;}
    cursor=func_8001FF84(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,cursor,x,y,&ovl666_D_80116EE0,0x7FA4,((wave+1)*2)|1);
    cursor=func_8001FCC8(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,cursor,mode);
    scratch->cursor=cursor;
}
