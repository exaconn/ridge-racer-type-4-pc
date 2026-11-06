#include "rr/overlay667_init.h"
#ifndef RR_GPU_PROJECTION_SCRATCH
#define RR_GPU_PROJECTION_SCRATCH ((RrProjectionScratchPrefix *)0x1F800000)
#endif

void ovl667_func_80117EF8(void)
{
    s32 choice;
    D_800F2DE4=0;
    ScreenFrame=0;
    D_800AC64A=0;
    D_800AC5E0=3;
    D_8010C068=0;
    choice=func_80032E9C(D_800AC648);
    D_800F3914=choice;
    func_8003A198();
    RR_GPU_PROJECTION_SCRATCH->word18=512;
    RR_GPU_PROJECTION_SCRATCH->word0C=-416;
    RR_GPU_PROJECTION_SCRATCH->word1C=0;
    RR_GPU_PROJECTION_SCRATCH->word20=0;
    RR_GPU_PROJECTION_SCRATCH->word08=0;
    RR_GPU_PROJECTION_SCRATCH->word10=0;
    D_80107198.words20[0]=0;
    D_80107198.words20[1]=0;
    D_80107198.words20[2]=0;
    D_800AC5D4=0;
    D_8010C0B8=0;
    D_800ACC18=D_800F4D70.second;
}
