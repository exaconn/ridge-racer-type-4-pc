/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "rr/trig.h"
#include "psyq/libgte/gtemat7.h"
#include "psyq/libgte/gtetrig.h"
#include "rr/menu_overlay_runtime.h"

/* extern */

void func_80090594(s32 arg0, s32 arg1);

s32 rcos(s32 angle) {
    s32 a=angle;
    s32 result;
    if(a<0) a=(s32)(0U-(u32)a);
    a &= 0xFFF;
    if(a<0x801) {
        if(a<0x401) {result=D_800A68C0[0x400-a];goto out;}
        result=D_800A60C0[a];result=-result;goto out;
    }
    if(a<0xC01) goto negative;
    result=D_800A50C0[a];goto out;
negative:
    result=-D_800A68C0[0xC00-a];
out:
    return result;
}

/* extern */
void func_80090594(s32 arg0, s32 arg1) {
    func_8009125C((s32) -(arg0 * 0x140) / arg1);
    func_80091268(0x01400000);
}
