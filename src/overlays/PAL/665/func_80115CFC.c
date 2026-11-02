#include "rr/overlay665_panels.h"
void ovl665_func_80115CFC(void) {
    s32 i,x;
    const RrUiUvRect *glyph;
    void *cursor;
    RrGpuSingleCommandPacket *packet;
    switch(ovl665_D_80119768.layout0C) {
    case 0:
        ovl665_func_80115C8C();
        func_8005B2D4(0);
        ovl665_D_80119768.layout0C=(s32)((u32)ovl665_D_80119768.layout0C+1U);
        break;
    case 1:
        if(D_800F3AFC[0].state10==3)ovl665_D_80119768.layout0C=2;
        break;
    case 2:
        if(D_800F3A60) {
            func_8004F24C(1);
            D_800F3AFC[0].state10=4;
            D_800AC5D0=64;
            ovl665_D_80119768.layout0C=(s32)((u32)ovl665_D_80119768.layout0C+1U);
        }
        break;
    case 3:
        D_800AC5D0--;
        func_800207D4(((RrFrameOrderingPrefix*)D_800ACC14)->ordering70,(64-D_800AC5D0)*4,69);
        if(D_800AC5D0<=0 && D_800F3AFC[0].state10==11) {
            ovl665_func_80114A24();
            ovl665_D_80119768.layout0C=0;
            ovl665_D_80119768.scene00=(s32)((u32)ovl665_D_80119768.scene00-1U);
            return;
        }
        break;
    }
    cursor=RR_PROJECTION_SCRATCH->pointers.cursor;
    i=0;
    glyph=&ovl665_D_80119028;
    x=24;
    for(;i<8;x+=36,i++) {
        cursor=func_8001FF84(((RrFrameOrderingPrefix*)D_800ACC14)->ordering70,cursor,x,144,glyph,0x7F38,((7-i)*223)/7+32);
    }
    packet=(RrGpuSingleCommandPacket*)cursor;
    RR_PROJECTION_SCRATCH->pointers.cursor=(u8*)packet;
    ((u8*)&packet->tag)[3]=1;
    {
        RrFrameOrderingPrefix *context=(RrFrameOrderingPrefix*)D_800ACC14;
        packet->command=0xE1000239U;
        packet->tag=(packet->tag&0xFF000000U)|(context->ordering70[0]&0xFFFFFFU);
        context->ordering70[0]=(context->ordering70[0]&0xFF000000U)|((u32)(unsigned long)packet&0xFFFFFFU);
        packet++;
        RR_PROJECTION_SCRATCH->pointers.cursor=(u8*)packet;
    }
    for(i=0;i<8;i++) {
        func_80057040(0,28+i*36,192,128);
        func_80057040(i+1,34+i*36,192,128);
    }
    packet=(RrGpuSingleCommandPacket*)RR_PROJECTION_SCRATCH->pointers.cursor;
    ((u8*)&packet->tag)[3]=1;
    {
        RrFrameOrderingPrefix *context=(RrFrameOrderingPrefix*)D_800ACC14;
        packet->command=0xE1000238U;
        packet->tag=(packet->tag&0xFF000000U)|(context->ordering70[0]&0xFFFFFFU);
        context->ordering70[0]=(context->ordering70[0]&0xFF000000U)|((u32)(unsigned long)packet&0xFFFFFFU);
        packet++;
        RR_PROJECTION_SCRATCH->pointers.cursor=(u8*)packet;
    }
    D_800AC590->b4=27;
    func_8003D6F0(0);
    if(D_800F3AFC[0].state10==3) {
        func_80032A14(D_800F3AFC[0].x04+10,D_800F3AFC[0].y06+40,D_800AC590);
    }
    func_8002A344(0x1007C);
}
