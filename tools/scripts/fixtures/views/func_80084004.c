/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/spu_tick.h"

void func_80084004(s32 mode)
{
    s32 system=func_8008BDF8();
    /* Retain the real active-field address in its configuration phase. */
    register u8 *base __asm__("$2");
    if(mode&0x1000) {
        /* Both word accesses remain inside the complete timer owner. */
        base=(u8 *)&D_800A4D38+4;
        *(s32 *)base=1;
        *(s32 *)(base-4)=mode&0xFFF;
    } else {
        /* Both word accesses remain inside the complete timer owner. */
        base=(u8 *)&D_800A4D38+4;
        *(s32 *)base=0;
        *(s32 *)(base-4)=mode;
    }
    if(D_800A4D38.selector<6) {
        switch(D_800A4D38.selector) {
        case 4:
            D_8010F064=50;
            if(system==1) D_800A4D38.selector=5;
            else D_800A4D38.selector=50;
            break;
        case 1:
            D_8010F064=60;
            if(system==0) D_800A4D38.selector=5;
            else D_800A4D38.selector=60;
            break;
        case 3:D_8010F064=120;break;
        case 2:D_8010F064=240;break;
        case 5:
            if(system==0) D_8010F064=60;
            else if(system==1) D_8010F064=50;
            else D_8010F064=60;
            break;
        case 0:
            if(system==0) D_8010F064=60;
            else if(system==1) D_8010F064=50;
            else D_8010F064=60;
            break;
        default:D_8010F064=60;
        }
    } else D_8010F064=D_800A4D38.selector;
}
