#include "rr/overlay660_course_layout.h"
void ovl660_func_80114920(void) {
    RrProjectionScratchPrefix *scratch;
    s32 selection;
    s16 *result;
    D_800F2DE4=0;
    D_800ABEB0=0;
    D_800AC64A=0;
    D_800AC5E0=3;
    selection=func_80032E9C(D_800AC648);
    result=&D_800F3914;
    *result=selection;
    func_8003A198();
    D_800F4D70.second=D_800102F0+0x2D000;
    func_8003A1B4();
    D_800ACC18=D_800F4D70.second;
    func_8003E928(0,12,64,68,32,0,0,1,0,83);
    func_8003E928(1,12,104,68,32,0,0,1,0,102);
    func_8003E928(2,12,144,68,32,0,0,1,0,85);
    func_8003E928(3,12,368,208,32,0,0,8,4,D_800AC648&7);
    func_8003E928(4,24,88,68,32,0,0,1,0,84);
    func_8003E928(5,24,128,68,32,0,0,1,0,54);
    func_8003E928(6,24,168,68,32,0,0,1,0,85);
    func_8003E928(7,24,88,68,32,0,0,1,0,84);
    func_8003E928(8,24,128,68,32,0,0,1,0,53);
    func_8003E928(9,24,168,68,32,0,0,1,0,85);
    func_8003E928(10,116,88,28,32,0,0,1,0,55);
    func_8003E928(11,116,128,28,32,0,0,1,0,56);
    scratch=RR_PROJECTION_SCRATCH;
    scratch->word18=512;
    scratch->word0C=-416;
    scratch->word1C=0;
    scratch->word20=0;
    scratch->word08=0;
    scratch->word10=0;
    D_800ABEF8[0].pos20[0]=0;
    D_800ABEF8[0].pos20[1]=0;
    D_800ABEF8[0].pos20[2]=0;
}
