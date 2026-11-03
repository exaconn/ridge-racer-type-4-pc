#include "rr/overlay665_visual.h"
void ovl665_func_801169BC(s32 arg0)
{
    u16 pts[4];

    pts[0] = (func_80090424(arg0) / 128) + ovl665_D_80119768.centerX38;
    pts[1] = (-func_800904F4(arg0) / 64) + ovl665_D_80119768.centerY3A;
    pts[2] = ((func_80090424(arg0) * 40) / 8192) + ovl665_D_80119768.centerX38;
    pts[3] = ((-func_800904F4(arg0) * 40) / 4096) + ovl665_D_80119768.centerY3A;
    func_8004D290((const s16 *)pts, ovl665_D_8011973C, 0, 0);
}
