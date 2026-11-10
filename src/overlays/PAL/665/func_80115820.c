#include "game/system/pad.h"
#include "rr/overlay665_calibration.h"
#include "rr/frame_storage_types.h"
void ovl665_func_80115820(void) {
    s32 i;
    void *cursor;
    GpuSingleCommandPacket *packet;
    s16 timer;
    FrameStorage *context;
    switch(ovl665_D_80119768.layout0C) {
    case 0:
        ovl665_func_80115798();
        SetPresentationDrawEnabled(0);
        ovl665_D_80119768.layout0C=(ovl665_D_80119768.layout0C+1U);
        break;
    case 1:
        if(BadgeRows[0].state10==3)ovl665_D_80119768.layout0C=2;
        break;
    case 2:
        if(PlayerControllers[0].edges08.fields.repeat&0x8000)Options.display.x34=(Options.display.x34-1U);
        if(PlayerControllers[0].edges08.fields.repeat&0x2000)Options.display.x34=(Options.display.x34+1U);
        if(PlayerControllers[0].edges08.fields.repeat&0x1000)Options.display.y38=(Options.display.y38-2U);
        if(PlayerControllers[0].edges08.fields.repeat&0x4000)Options.display.y38=(Options.display.y38+2U);
        if(Options.display.x34 < -10)Options.display.x34=-10;
        if(Options.display.x34 >=11)Options.display.x34=10;
        if(Options.display.y38<0)Options.display.y38=0;
        if(Options.display.y38>=33)Options.display.y38=32;
        if(PlayerControllers[0].edges08.fields.pressed&0x800) {
            Options.display.x34=0;
            Options.display.y38=16;
        } else if(PlayerControllers[0].action30) {
            PlaySfx(1);
            BadgeRows[0].state10=4;
            FadeCountdown=64;
            ovl665_D_80119768.layout0C=(ovl665_D_80119768.layout0C+1U);
        }
        for(i=0;i<2;i++) {
            DrawBuffers[i].environment.display5C.screen.x=(s16)Options.display.x34;
            DrawBuffers[i].environment.display5C.screen.y=(s16)Options.display.y38;
        }
        break;
    case 3:
        timer=(u16)FadeCountdown-1;
        FadeCountdown=timer;
        AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,(64-timer)*4,69);
        if(FadeCountdown<=0 && BadgeRows[0].state10==11) {
            ovl665_func_80114A24();
            ovl665_D_80119768.layout0C=0;
            ovl665_D_80119768.scene00=(ovl665_D_80119768.scene00-1U);
            return;
        }
        break;
    }
    cursor=RR_PROJECTION_SCRATCH->pointers.cursor;
    for(i=0;i<2;i++) {
        cursor=AddSpritePrim(CurrentDrawBuf->buffer.ordering70.words,cursor,160,128+i*96,&ovl665_D_80119018[i],0x7F38,255);
    }
    for(i=0;i<2;i++) {
        cursor=AddSpritePrim(CurrentDrawBuf->buffer.ordering70.words,cursor,136+i*48,176,&ovl665_D_80119020[i],0x7F38,255);
    }
    packet=(GpuSingleCommandPacket *)cursor;
    RR_PROJECTION_SCRATCH->pointers.cursor=(u8 *)packet;
    ((u8 *)&packet->tag)[3]=1;
    context=CurrentDrawBuf;
    packet->command=0xE1000259;
    packet->tag=(packet->tag&0xFF000000U)|(context->buffer.ordering70.words[0]&0xFFFFFFU);
    context->buffer.ordering70.words[0]=(context->buffer.ordering70.words[0]&0xFF000000U)|((unsigned long)packet&0xFFFFFFU);
    packet++;
    RR_PROJECTION_SCRATCH->pointers.cursor=(u8 *)packet;
    CardDialogueStatePtr->b4=26;
    UpdateBadgeRowAnim(0);
    if(BadgeRows[0].state10==3)UpdateDialogue(BadgeRows[0].x04+10,BadgeRows[0].y06+40,CardDialogueStatePtr);
    UpdateScreenFade(0x1007A);
}
