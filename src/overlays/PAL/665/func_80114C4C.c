#include "game/system/pad.h"
#include "rr/overlay665_menu.h"
#include "rr/frame_storage_types.h"
void ovl665_update_menu(void) {
    s32 i;
    /* Preserve the original signed two-stage range check. */
    s32 choice;
    switch(ovl665_options_state.layout0C) {
    case 0:
        if(FadeCountdown>0) {
            FadeCountdown--;
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[702],FadeCountdown*4,69);
        }
        if(UiRows[0].byte14==2 && FadeCountdown==0) {
            UiRows[ovl665_options_state.selection08].cells[0]=0;
            StepMenuValue(0,2,(char*)&ovl665_options_state.selection08,7,0);
            UiRows[ovl665_options_state.selection08].cells[0]=2;
            if(PlayerControllers[0].action28) {
                if(ovl665_options_state.selection08==1 && ovl665_detect_controller_kind(1)<0) {
                    PlaySfx(4);
                } else {
                    PlaySfx(1);
                    UiRows[ovl665_options_state.selection08].cells[0]=1;
                    FadeCountdown=64;
                    ovl665_options_state.layout0C=(ovl665_options_state.layout0C+1U);
                }
            } else if(PlayerControllers[0].action2C) {
                PlaySfx(2);
                ovl665_options_state.selection08=6;
                FadeCountdown=64;
                ovl665_options_state.layout0C=(ovl665_options_state.layout0C+1U);
            }
        }
        SetPresentationDrawEnabled(1);
        break;
    case 1:
        for(i=0;i<8;i++) {
            if(UiRows[i].byte14==2)UiRows[i].byte14=3;
        }
        if(ovl665_options_state.selection08>=5 || ovl665_options_state.selection08==3) {
            SeqFadeVolume(0,(s16)(FadeCountdown-4));
        }
        ovl665_options_state.layout0C=(ovl665_options_state.layout0C+1U);
        break;
    case 2:
        FadeCountdown--;
        AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0],(64-FadeCountdown)*4,69);
        if(FadeCountdown<=0) {
            SetPresentationDrawEnabled(0);
            ovl665_options_state.exit34.word=0;
            ovl665_options_state.layout0C=0;
            ovl665_options_state.detail10=0;
            ovl665_options_state.scene00=(ovl665_options_state.scene00+1U);
            choice=ovl665_options_state.selection08;
            if(choice<2) {
                if(choice>=0)ovl665_init_controller_options();
            }
        }
        break;
    }
    if(ovl665_detect_controller_kind(1)<0) {
        UiRows[1].half20=1;
        if(ovl665_options_state.selection08==1)UiRows[1].cells[0]=4;
    } else {
        UiRows[1].half20=0;
    }
    for(i=0;i<8;i++)DrawUiRow(i);
    UpdateScreenFade(0x10066);
}
