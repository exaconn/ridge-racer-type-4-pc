#include "rr/overlay662_progress_init.h"

void ovl662_func_80114998(void)
{
    s32 i,j,k;
    D_800F2DE4=0; ScreenFrame=0; D_800AC64A=0; D_800AC5E0=3;
    func_8003A198();
    D_800F4D70.second=D_800102F0+0x2D000;
    func_8003A1B4();
    D_800ACC18=D_800F4D70.second;
    func_8003E928(0,118,176,68,32,0,0,1,0,54);
    func_8003E928(1,118,216,68,32,0,0,1,1,15);
    func_8003E928(2,118,256,68,32,0,0,1,0,85);
    func_80013890(5);
    ovl662_D_801272FC=0;
    D_800ACC18=D_800F4D40;
    D_800F4D70.first=D_800F4D40;
    for(i=0;i<4;i++) {
        ovl662_D_801272D8[i]=0;
        for(j=0;j<4;j++) {
            ovl662_D_80127328[i][j]=0;
            for(k=0;k<20;k++)
                ovl662_D_80127328[i][j]+=(D_800F3920.prefix.progress.progress10[i][j]>>k)&1;
            ovl662_D_801272D8[i]+=ovl662_D_80127328[i][j];
        }
        ovl662_D_801272FC+=ovl662_D_801272D8[i];
    }
}
