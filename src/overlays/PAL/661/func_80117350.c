#include "rr/overlay661_init.h"
#ifndef RR_GPU_PROJECTION_SCRATCH
#define RR_GPU_PROJECTION_SCRATCH ((RrProjectionScratchPrefix *)0x1F800000)
#endif

void ovl661_func_80117350(void)
{
    D_800F2DE4=0;ScreenFrame=0;MenuCursor=0;MenuItemCount=4;
    D_800ACBD0[2].first=(func_80096464()&3)+10;
    D_800ACBD0[3].first=(func_80096464()&3)+12;
    D_800F3914=DecodeCourseIndex(D_800AC648);
    ResetScreenCounters();
    RR_GPU_PROJECTION_SCRATCH->word18=512;
    RR_GPU_PROJECTION_SCRATCH->word0C=-416;
    RR_GPU_PROJECTION_SCRATCH->word1C=0;
    RR_GPU_PROJECTION_SCRATCH->word20=0;
    RR_GPU_PROJECTION_SCRATCH->word08=0;
    RR_GPU_PROJECTION_SCRATCH->word10=0;
    ComCars.words20[0]=0;ComCars.words20[1]=0;ComCars.words20[2]=0;
    D_800AC5D4=0;
    LoadCursor=D_800F4D70.second;
    func_8002AF8C(0,1,1);
}
