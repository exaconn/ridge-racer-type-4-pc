#include "rr/menu_choice.h"

void StepResultsMenuChoice(void) {
    s32 state;
    u32 selected=D_800F4D99;
    SelectedMakerIndex=selected;
    ((u8 *)UiRows)[12+selected]=0;
    if(D_800F3A42&0x2000) {
        if(SelectedMakerIndex<3) {

            SelectedMakerIndex=SelectedMakerIndex+1;
            MenuStepDirection=2;
            goto changed;
        }
    }
    if(D_800F3A42&0x8000) {
        if(SelectedMakerIndex>0) {
            SelectedMakerIndex=SelectedMakerIndex-1;
            MenuStepDirection=1;
            goto changed;
        }
    }
    MenuStepDirection=0;
    goto active;
changed:
    /* The sound helper can change selection and backing pointers. */
    PlaySfx(0);
    if(SelectedMakerIndex<0)SelectedMakerIndex=0;
    else if(SelectedMakerIndex>=4)SelectedMakerIndex=3;
    Dialogue[4]=(u8)SelectedMakerIndex+34;
    D_800F4D99=SelectedMakerIndex;
    GlyphRows[SelectedMakerIndex].control02=-6;
    GlyphRows[SelectedMakerIndex].flags00=0;
    UiRows->byte16=(u8)SelectedMakerIndex+4;
active:
    {
        u8 *ui=(u8 *)UiRows;
        s32 active;

        active=SelectedMakerIndex;
        *(u8 *)((unsigned long)ui+(unsigned long)active+12UL)=2;
    }
    if(D_800F3A60) {
        state=BadgeRows->state10;
        if(state==3) {
            PlaySfx(1);
            {
                MenuSettings2C *settings=&SelectionState;
                BadgeRow1C *control=BadgeRows;
                UiRow24 *ui=UiRows;

                /* Four copies of the same byte-aligned source record. */
                settings->copies[0]=SelectionRows;
                settings->copies[1]=SelectionRows;
                settings->copies[2]=SelectionRows;
                settings->copies[3]=SelectionRows;
                control->state10=4;
                ui->byte14=state;
                {
                    u32 byte=SelectionRows.bytes[1];
                    s32 mode;

                    mode=2;
                    settings->mode1A=mode;
                    settings->copies[0].bytes[1]=byte;
                }
            }
            UpdateUnlockedCarMask();
            ResetScreenCounters();
            {
                s32 zero=0;
                void *resource;
                s32 one;
                s32 last;

                /* Both preceding helpers may replace the resource buffer. */
                resource=MenuArenaBase;

                LoadCursor=resource;
                one=1;

                last=zero;
                SetMenuFrameOptions(zero,one,last);
            }
        }
    }
}
