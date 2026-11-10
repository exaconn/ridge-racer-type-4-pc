#include "rr/overlay661_init.h"
#include "rr/load_cursor.h"
#ifndef RR_GPU_PROJECTION_SCRATCH
#define RR_GPU_PROJECTION_SCRATCH ((ProjectionScratchPrefix *)0x1F800000)
#endif

void ovl661_func_80117350(void)
{
    ModeFrame=0;ScreenFrame=0;MenuCursor=0;MenuItemCount=4;
    PlayerCourseChoices[2].first=(PsyqRand()&3)+10;
    PlayerCourseChoices[3].first=(PsyqRand()&3)+12;
    CourseIndex=DecodeCourseIndex(D_800AC648);
    ResetScreenCounters();
    RR_GPU_PROJECTION_SCRATCH->pose08.angles10.xyz.x=512;
    RR_GPU_PROJECTION_SCRATCH->pose08.position00.words[1]=-416;
    RR_GPU_PROJECTION_SCRATCH->pose08.angles10.xyz.y=0;
    RR_GPU_PROJECTION_SCRATCH->pose08.angles10.xyz.z=0;
    RR_GPU_PROJECTION_SCRATCH->pose08.position00.words[0]=0;
    RR_GPU_PROJECTION_SCRATCH->pose08.position00.words[2]=0;
    ComCars.words20[0]=0;ComCars.words20[1]=0;ComCars.words20[2]=0;
    D_800AC5D4=0;
    LoadCursor=MenuArenas.second;
    SetMenuFrameOptions(0,1,1);
}
