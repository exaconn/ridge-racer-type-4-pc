#include "rr/overlay665_menu.h"
void ovl665_func_80114C4C(void) {
    s32 i;
    /* Preserve the original signed two-stage range check. */
    s32 choice;
    switch(ovl665_D_80119768.layout0C) {
    case 0:
        if(D_800AC5D0>0) {
            D_800AC5D0--;
            func_800207D4(&((RrFrameOrderingPrefix*)D_800ACC14)->ordering70[702],D_800AC5D0*4,69);
        }
        if(UiRows[0].byte14==2 && D_800AC5D0==0) {
            UiRows[ovl665_D_80119768.selection08].cells[0]=0;
            StepMenuValue(0,2,(char*)&ovl665_D_80119768.selection08,7,0);
            UiRows[ovl665_D_80119768.selection08].cells[0]=2;
            if(D_800F3A38[0].a) {
                if(ovl665_D_80119768.selection08==1 && ovl665_func_8011629C(1)<0) {
                    PlaySfx(4);
                } else {
                    PlaySfx(1);
                    UiRows[ovl665_D_80119768.selection08].cells[0]=1;
                    D_800AC5D0=64;
                    ovl665_D_80119768.layout0C=(s32)((u32)ovl665_D_80119768.layout0C+1U);
                }
            } else if(D_800F3A38[0].b) {
                PlaySfx(2);
                ovl665_D_80119768.selection08=6;
                D_800AC5D0=64;
                ovl665_D_80119768.layout0C=(s32)((u32)ovl665_D_80119768.layout0C+1U);
            }
        }
        func_8005B2D4(1);
        break;
    case 1:
        for(i=0;i<8;i++) {
            if(UiRows[i].byte14==2)UiRows[i].byte14=3;
        }
        if(ovl665_D_80119768.selection08>=5 || ovl665_D_80119768.selection08==3) {
            func_80053AF4(0,(s16)(D_800AC5D0-4));
        }
        ovl665_D_80119768.layout0C=(s32)((u32)ovl665_D_80119768.layout0C+1U);
        break;
    case 2:
        D_800AC5D0--;
        func_800207D4(&((RrFrameOrderingPrefix*)D_800ACC14)->ordering70[0],(64-D_800AC5D0)*4,69);
        if(D_800AC5D0<=0) {
            func_8005B2D4(0);
            ovl665_D_80119768.exit34.word=0;
            ovl665_D_80119768.layout0C=0;
            ovl665_D_80119768.detail10=0;
            ovl665_D_80119768.scene00=(s32)((u32)ovl665_D_80119768.scene00+1U);
            choice=ovl665_D_80119768.selection08;
            if(choice<2) {
                if(choice>=0)ovl665_func_80116070();
            }
        }
        break;
    }
    if(ovl665_func_8011629C(1)<0) {
        UiRows[1].half20=1;
        if(ovl665_D_80119768.selection08==1)UiRows[1].cells[0]=4;
    } else {
        UiRows[1].half20=0;
    }
    for(i=0;i<8;i++)DrawUiRow(i);
    UpdateScreenFade(0x10066);
}
