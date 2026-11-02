#include "rr/overlay_menu_init.h"

void ovl663_func_80115028(s32 mode)
{
    s32 one=1;
    D_800F2DE4=0;D_800ABEB0=0;D_800AC648=0;D_800F3914=0;D_800AC64A=0;ovl663_D_80116898=0;
    func_8002A604();
    func_8003E928(0,12,64,68,32,0,0,one,one,24);
    func_8003E928(1,12,104,68,32,0,0,one,0,85);
    func_8003E928(2,12,368,208,32,0,0,8,4,0);
    func_8003E928(3,24,88,68,32,0,0,one,0,83);
    if (mode==2)func_8003D228(one,49,188,222,152,0,0xB04);
    else func_8003D228(one,49,188,222,152,0,0xA04);
    D_800F3AFC[1].half02=one;
}
