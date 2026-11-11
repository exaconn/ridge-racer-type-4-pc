#include "types.h"
#include "rr/trig.h"
#include "psyq/libgte/gtemat7.h"
#include "psyq/libgte/gtetrig.h"
#include "rr/menu_overlay_runtime.h"

/* extern */

void SetFogNear(s32 arg0, s32 arg1);

s32 rcos(s32 angle) {
    s32 a=angle;
    s32 result;
    if(a<0) a=(0U-a);
    a &= 0xFFF;
    if(a<0x801) {
        if(a<0x401) {result=SinTable[0x400-a];goto out;}
        result=D_800A60C0[a];result=-result;goto out;
    }
    if(a<0xC01) goto negative;
    result=D_800A50C0[a];goto out;
negative:
    result=-SinTable[0xC00-a];
out:
    return result;
}

/* extern */
void SetFogNear(s32 arg0, s32 arg1) {
    SetDQA((s32) -(arg0 * 0x140) / arg1);
    SetDQB(0x01400000);
}
