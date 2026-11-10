#include "rr/overlay665_panels.h"
#include "rr/frame_storage_types.h"
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
        if(D_800F3A60) {
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
