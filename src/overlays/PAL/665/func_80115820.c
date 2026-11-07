#include "rr/overlay665_calibration.h"
void ovl665_func_80115820(void) {
    s32 i;
    void *cursor;
    RrGpuSingleCommandPacket *packet;
    s16 timer;
    RrFrameOrderingPrefix *context;
    switch(ovl665_D_80119768.layout0C) {
    case 0:
        ovl665_func_80115798();
        func_8005B2D4(0);
        ovl665_D_80119768.layout0C=(s32)((u32)ovl665_D_80119768.layout0C+1U);
        break;
    case 1:
        if(D_800F3AFC[0].state10==3)ovl665_D_80119768.layout0C=2;
        break;
    case 2:
        if(D_800F3A38[0].buttons0A&0x8000)D_800AD528.display.x34=(s32)((u32)D_800AD528.display.x34-1U);
        if(D_800F3A38[0].buttons0A&0x2000)D_800AD528.display.x34=(s32)((u32)D_800AD528.display.x34+1U);
        if(D_800F3A38[0].buttons0A&0x1000)D_800AD528.display.y38=(s32)((u32)D_800AD528.display.y38-2U);
        if(D_800F3A38[0].buttons0A&0x4000)D_800AD528.display.y38=(s32)((u32)D_800AD528.display.y38+2U);
        if(D_800AD528.display.x34 < -10)D_800AD528.display.x34=-10;
        if(D_800AD528.display.x34 >=11)D_800AD528.display.x34=10;
        if(D_800AD528.display.y38<0)D_800AD528.display.y38=0;
        if(D_800AD528.display.y38>=33)D_800AD528.display.y38=32;
        if(D_800F3A38[0].buttons08&0x800) {
            D_800AD528.display.x34=0;
            D_800AD528.display.y38=16;
        } else if(D_800F3A38[0].accept30) {
            PlaySfx(1);
            D_800F3AFC[0].state10=4;
            D_800AC5D0=64;
            ovl665_D_80119768.layout0C=(s32)((u32)ovl665_D_80119768.layout0C+1U);
        }
        for(i=0;i<2;i++) {
            D_800ADAF0[i].environment.screen.x=(s16)D_800AD528.display.x34;
            D_800ADAF0[i].environment.screen.y=(s16)D_800AD528.display.y38;
        }
        break;
    case 3:
        timer=(u16)D_800AC5D0-1;
        D_800AC5D0=timer;
        func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,(64-(s32)timer)*4,69);
        if(D_800AC5D0<=0 && D_800F3AFC[0].state10==11) {
            ovl665_func_80114A24();
            ovl665_D_80119768.layout0C=0;
            ovl665_D_80119768.scene00=(s32)((u32)ovl665_D_80119768.scene00-1U);
            return;
        }
        break;
    }
    cursor=RR_PROJECTION_SCRATCH->pointers.cursor;
    for(i=0;i<2;i++) {
        cursor=func_8001FF84(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,cursor,160,128+i*96,&ovl665_D_80119018[i],0x7F38,255);
    }
    for(i=0;i<2;i++) {
        cursor=func_8001FF84(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70,cursor,136+i*48,176,&ovl665_D_80119020[i],0x7F38,255);
    }
    packet=(RrGpuSingleCommandPacket *)cursor;
    RR_PROJECTION_SCRATCH->pointers.cursor=(u8 *)packet;
    ((u8 *)&packet->tag)[3]=1;
    context=(RrFrameOrderingPrefix *)D_800ACC14;
    packet->command=0xE1000259;
    packet->tag=(packet->tag&0xFF000000U)|(context->ordering70[0]&0xFFFFFFU);
    context->ordering70[0]=(context->ordering70[0]&0xFF000000U)|((u32)(unsigned long)packet&0xFFFFFFU);
    packet++;
    RR_PROJECTION_SCRATCH->pointers.cursor=(u8 *)packet;
    D_800AC590->b4=26;
    func_8003D6F0(0);
    if(D_800F3AFC[0].state10==3)func_80032A14(D_800F3AFC[0].x04+10,D_800F3AFC[0].y06+40,D_800AC590);
    func_8002A344(0x1007A);
}
