#include "rr/overlay665_calibration.h"
#include "game/system/pad.h"
#include "rr/frame_storage_types.h"
#include "rr/overlay665_panels.h"

void ovl665_init_screen_adjust(void)
{
    ClearScreenOverlayState();
    MenuCursor = 0;
    InitBadgeRow(0, 12, 304, 296, 88, 0, 0);
    BadgeRows[0].half02 = 1;
    SetMenuFrameOptions(1, 1, 0);
    DebugPrintStub(ovl665_screen_offset_debug_fmt, Options.display.x34, Options.display.y38);
}

void ovl665_update_screen_adjust(void) {
    s32 i;
    void *cursor;
    GpuSingleCommandPacket *packet;
    s16 timer;
    FrameStorage *context;
    switch(ovl665_options_state.layout0C) {
    case 0:
        ovl665_init_screen_adjust();
        SetPresentationDrawEnabled(0);
        ovl665_options_state.layout0C=(ovl665_options_state.layout0C+1U);
        break;
    case 1:
        if(BadgeRows[0].state10==3)ovl665_options_state.layout0C=2;
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
            ovl665_options_state.layout0C=(ovl665_options_state.layout0C+1U);
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
            ovl665_init_menu();
            ovl665_options_state.layout0C=0;
            ovl665_options_state.scene00=(ovl665_options_state.scene00-1U);
            return;
        }
        break;
    }
    cursor=RR_PROJECTION_SCRATCH->pointers.cursor;
    for(i=0;i<2;i++) {
        cursor=AddSpritePrim(CurrentDrawBuf->buffer.ordering70.words,cursor,160,128+i*96,&ovl665_screen_frame_uv_rects[i],0x7F38,255);
    }
    for(i=0;i<2;i++) {
        cursor=AddSpritePrim(CurrentDrawBuf->buffer.ordering70.words,cursor,136+i*48,176,&ovl665_screen_arrow_uv_rects[i],0x7F38,255);
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

void ovl665_init_brightness_panel(void)
{
    ClearScreenOverlayState();
    ScreenFadeState[0] = 2;
    InitBadgeRow(0, 12, 304, 296, 88, 0, 0);
    BadgeRows[0].half02 = 1;
    SetMenuFrameOptions(1, 1, 0);
}

void ovl665_update_brightness_panel(void) {
    s32 i,x;
    const UiUvRect *glyph;
    void *cursor;
    GpuSingleCommandPacket *packet;
    switch(ovl665_options_state.layout0C) {
    case 0:
        ovl665_init_brightness_panel();
        SetPresentationDrawEnabled(0);
        ovl665_options_state.layout0C=(ovl665_options_state.layout0C+1U);
        break;
    case 1:
        if(BadgeRows[0].state10==3)ovl665_options_state.layout0C=2;
        break;
    case 2:
        if(MenuReturnSubmodeIndex) {
            PlaySfx(1);
            BadgeRows[0].state10=4;
            FadeCountdown=64;
            ovl665_options_state.layout0C=(ovl665_options_state.layout0C+1U);
        }
        break;
    case 3:
        FadeCountdown--;
        AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,(64-FadeCountdown)*4,69);
        if(FadeCountdown<=0 && BadgeRows[0].state10==11) {
            ovl665_init_menu();
            ovl665_options_state.layout0C=0;
            ovl665_options_state.scene00=(ovl665_options_state.scene00-1U);
            return;
        }
        break;
    }
    cursor=RR_PROJECTION_SCRATCH->pointers.cursor;
    i=0;
    glyph=&ovl665_brightness_swatch_uv_rect;
    x=24;
    for(;i<8;x+=36,i++) {
        cursor=AddSpritePrim(CurrentDrawBuf->buffer.ordering70.words,cursor,x,144,glyph,0x7F38,((7-i)*223)/7+32);
    }
    packet=(GpuSingleCommandPacket*)cursor;
    RR_PROJECTION_SCRATCH->pointers.cursor=(u8*)packet;
    ((u8*)&packet->tag)[3]=1;
    {
        FrameStorage *context=CurrentDrawBuf;
        packet->command=0xE1000239U;
        packet->tag=(packet->tag&0xFF000000U)|(context->buffer.ordering70.words[0]&0xFFFFFFU);
        context->buffer.ordering70.words[0]=(context->buffer.ordering70.words[0]&0xFF000000U)|((unsigned long)packet&0xFFFFFFU);
        packet++;
        RR_PROJECTION_SCRATCH->pointers.cursor=(u8*)packet;
    }
    for(i=0;i<8;i++) {
        DrawHudDigit(0,28+i*36,192,128);
        DrawHudDigit(i+1,34+i*36,192,128);
    }
    packet=(GpuSingleCommandPacket*)RR_PROJECTION_SCRATCH->pointers.cursor;
    ((u8*)&packet->tag)[3]=1;
    {
        FrameStorage *context=CurrentDrawBuf;
        packet->command=0xE1000238U;
        packet->tag=(packet->tag&0xFF000000U)|(context->buffer.ordering70.words[0]&0xFFFFFFU);
        context->buffer.ordering70.words[0]=(context->buffer.ordering70.words[0]&0xFF000000U)|((unsigned long)packet&0xFFFFFFU);
        packet++;
        RR_PROJECTION_SCRATCH->pointers.cursor=(u8*)packet;
    }
    CardDialogueStatePtr->b4=27;
    UpdateBadgeRowAnim(0);
    if(BadgeRows[0].state10==3) {
        UpdateDialogue(BadgeRows[0].x04+10,BadgeRows[0].y06+40,CardDialogueStatePtr);
    }
    UpdateScreenFade(0x1007C);
}
