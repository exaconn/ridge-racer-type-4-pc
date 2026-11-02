#include "rr/overlay663_time.h"

u8 *ovl663_func_80115EE8(s32 milliseconds)
{
    s32 seconds = milliseconds / 1000;
    s32 minutes = seconds / 60;
    s32 remainder = seconds % 60;
    seconds = milliseconds - seconds * 1000;
    ovl663_D_80116874[0] = minutes / 10 + '0';
    ovl663_D_80116874[1] = minutes % 10 + '0';
    ovl663_D_80116874[3] = remainder / 10 + '0';
    ovl663_D_80116874[4] = remainder % 10 + '0';
    ovl663_D_80116874[6] = seconds / 100 + '0';
    ovl663_D_80116874[7] = seconds / 10 % 10 + '0';
    ovl663_D_80116874[8] = seconds % 10 + '0';
    return ovl663_D_80116874;
}
