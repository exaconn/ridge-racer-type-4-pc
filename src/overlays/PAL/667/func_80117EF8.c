#include "rr/overlay667_init.h"
#include "rr/load_cursor.h"
#ifndef RR_GPU_PROJECTION_SCRATCH
#define RR_GPU_PROJECTION_SCRATCH ((ProjectionScratchPrefix *)0x1F800000)
#endif

void ovl667_init_course_menu(void)
{
    s32 choice;
    ModeFrame=0;
    ScreenFrame=0;
    MenuCursor=0;
    MenuItemCount=3;
    D_8010C068=0;
    choice=DecodeCourseIndex(SelectedCourseChoice);
    CourseIndex=choice;
    ResetScreenCounters();
    RR_GPU_PROJECTION_SCRATCH->pose08.angles10.xyz.x=512;
    RR_GPU_PROJECTION_SCRATCH->pose08.position00.words[1]=-416;
    RR_GPU_PROJECTION_SCRATCH->pose08.angles10.xyz.y=0;
    RR_GPU_PROJECTION_SCRATCH->pose08.angles10.xyz.z=0;
    RR_GPU_PROJECTION_SCRATCH->pose08.position00.words[0]=0;
    RR_GPU_PROJECTION_SCRATCH->pose08.position00.words[2]=0;
    ComCars.words20[0]=0;
    ComCars.words20[1]=0;
    ComCars.words20[2]=0;
    ActivePlayerIndex=0;
    D_8010C0B8=0;
    LoadCursor=MenuArenas.second;
}
