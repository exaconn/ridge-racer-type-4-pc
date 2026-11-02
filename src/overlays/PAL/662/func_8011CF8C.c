#include "rr/overlay662_update.h"

void ovl662_func_8011CF8C(void)
{
    s32 first,second;
    if(ovl662_D_801276A8.fade00>=0) {
        union {u32 bits;s32 value;} intensity;
        if(ovl662_D_801276A8.fade00!=0)ovl662_D_801276A8.fade00--;
        intensity.bits=(64U-(u32)ovl662_D_801276A8.fade00)<<2;
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,intensity.value,69);
    }
    if(ovl662_D_801276A8.flagsD1&1)func_8003D6F0(0);
    if(ovl662_D_801276A8.flagsD1&2){func_8003EC88(0);func_8003EC88(1);}
    if(ovl662_D_801276A8.flagsD1&4){func_80032DF0(D_800AC590);func_80032A14(22,320,D_800AC590);}
    first=0;
    if(ovl662_D_801276A8.fade00>=0)goto no_second;
    if(!(ovl662_D_801276A8.flagsD1&3))goto first_only;
    if(!(ovl662_D_801276A8.flagsD1&4))goto no_second;
    first=1;second=first;goto publish;
first_only:
    first=1;
no_second:
    /* ASM_BARRIER: retain the common second-flag assignment after the branch. */
    __asm__ volatile("" : : : "memory");
    second=0;
publish:
    func_8002AF8C(first,second,0);
    func_8002A344(0x1004E);
}
