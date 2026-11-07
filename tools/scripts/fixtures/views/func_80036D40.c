#include "rr/menu_choice.h"

void func_80036D40(void) {
    s32 state;
    u32 selected=D_800F4D99;
    D_800F3826=selected;
    ((u8 *)UiRows)[12+selected]=0;
    if(D_800F3A42&0x2000) {
        if(D_800F3826<3) {

            D_800F3826=D_800F3826+1;
            MenuStepDirection=2;
            goto changed;
        }
    }
    if(D_800F3A42&0x8000) {
        if(D_800F3826>0) {
            D_800F3826=D_800F3826-1;
            MenuStepDirection=1;
            goto changed;
        }
    }
    MenuStepDirection=0;
    goto active;
changed:
    /* The sound helper can change selection and backing pointers. */
    PlaySfx(0);
    if(D_800F3826<0)D_800F3826=0;
    else if(D_800F3826>=4)D_800F3826=3;
    D_800FF68C[4]=(u8)D_800F3826+34;
    D_800F4D99=D_800F3826;
    GlyphRows[D_800F3826].control02=-6;
    GlyphRows[D_800F3826].flags00=0;
    UiRows->byte16=(u8)D_800F3826+4;
active:
    {
        u8 *ui=(u8 *)UiRows;
        s32 active;

        active=D_800F3826;
        *(u8 *)((unsigned long)ui+(unsigned long)active+12UL)=2;
    }
    if(D_800F3A60) {
        state=BadgeRows->state10;
        if(state==3) {
            PlaySfx(1);
            {
                RrMenuSettings2C *settings=&D_800AD4D8;
                RrBadgeRow1C *control=BadgeRows;
                RrUiRow24 *ui=UiRows;

                /* Four copies of the same byte-aligned source record. */
                settings->copies[0]=D_800F4D98;
                settings->copies[1]=D_800F4D98;
                settings->copies[2]=D_800F4D98;
                settings->copies[3]=D_800F4D98;
                control->state10=4;
                ui->byte14=state;
                {
                    u32 byte=D_800F4D98.bytes[1];
                    s32 mode;

                    mode=2;
                    settings->mode1A=mode;
                    settings->copies[0].bytes[1]=byte;
                }
            }
            func_80039920();
            ResetScreenCounters();
            {
                s32 zero=0;
                void *resource;
                s32 one;
                s32 last;

                /* Both preceding helpers may replace the resource buffer. */
                resource=D_800F4D40;

                LoadCursor=resource;
                one=1;

                last=zero;
                func_8002AF8C(zero,one,last);
            }
        }
    }
}
