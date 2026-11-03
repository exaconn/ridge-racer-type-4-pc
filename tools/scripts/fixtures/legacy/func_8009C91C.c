/* GCC_VERSION: 970318 */
#include "rr/commands.h"
s32 func_8009C91C(s32 value) {
    s32 phase=D_800ABDF8;
    s32 result;
    if(phase==9) D_80114730=(u32)D_80114728+1;
    else if(phase>=11) D_80114718[phase-11]=value;
    result=0;
    return result;
}
