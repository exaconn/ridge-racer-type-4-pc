#include "game/gfx/model.h"
#include "rr/overlay662_progress_init.h"
#include "rr/load_cursor.h"
#include "rr/diagnostics.h"
#include "rr/overlay_menu_init.h"
#include "rr/menu_transition.h"
#include "rr/frame_environment.h"
#include "rr/save_progress.h"
#include "rr/course_choice_types.h"
#include "rr/string_tail_copy.h"
#include "game/system/pad.h"
#include "rr/overlay662_progress_controller.h"
#include "rr/frame_storage_types.h"

extern CourseChoice4 CourseChoiceTable[2][9];
void SeqStartTrack(s32 mode);
extern const char ovl662_str_init_garage_scene[];
void SetModelTexturePage(s32 mode);
extern s16 MenuCursor;
extern EmblemTimImage840 GarageDefaultCarTimBuf;
void StepMenuValue(s32 mode, s32 width, char *selection, s32 limit, s32 flags);
void SeqFadeVolume(s32, s32);
void SeqMuteVolume(void);
void UploadCarClut(void *, s32, s32);

void ovl662_init_progress_menu(void)
{
    s32 i,j,k;
    ModeFrame=0;
    ScreenFrame=0;
    MenuCursor=0;
    MenuItemCount=3;
    ResetScreenCounters();
    MenuArenas.second=CdDirectoryBuffer+0x2D000;
    InitMenuArenas();
    LoadCursor=MenuArenas.second;
    InitUiRow(0,118,176,68,32,0,0,1,0,54);
    InitUiRow(1,118,216,68,32,0,0,1,1,15);
    InitUiRow(2,118,256,68,32,0,0,1,0,85);
    SetupRenderer(5);
    ovl662_total_unlocked_cars=0;
    LoadCursor=MenuArenaBase;
    MenuArenas.first=MenuArenaBase;
    for(i=0;i<4;i++) {
        ovl662_team_unlocked_counts[i]=0;
        for(j=0;j<4;j++) {
            ovl662_team_maker_car_counts[i][j]=0;
            for(k=0;k<20;k++)
                ovl662_team_maker_car_counts[i][j]+=(SaveProgress.prefix.progress.progress10[i][j]>>k)&1;
            ovl662_team_unlocked_counts[i]+=ovl662_team_maker_car_counts[i][j];
        }
        ovl662_total_unlocked_cars+=ovl662_team_unlocked_counts[i];
    }
}

void ovl662_init_garage_scene(void) {
    SetDispMask(0);
    ClearScreenOverlayState();
    ovl662_init_progress_menu();
    SetupDrawDispEnvs(0, 0, 0, 2);
    SetModelTexturePage(25);
    ModeGroup = 1;
    ModeIndex = 40;
    SubmodeState = 0;
    DebugPrintStub(ovl662_str_init_garage_scene);
    if (SaveProgress.prefix.reserved50.words[0] == 1) {
        if (CourseChoiceTable[1][7].glyph >= 0) {
            CourseChoiceTable[1][8].glyph = 359;
            CourseChoiceTable[1][8].first = 1;
        }
    }
    SeqStartTrack(0);
}

void ovl662_update_progress_menu(void)
{
    s32 i;
    s32 state;
    s32 unused[2]; /* never referenced; original frame reserves 8 local bytes */

    ModeFrame++;
    switch ((u16)SubmodeState) {
    case 0:
        state = UiRows->byte14;
        if (state != 2) {
            break;
        }
        SetMenuFrameOptions(1, 1, 1);
        UiRows[MenuCursor].cells[0] = 0;
        StepMenuValue(0, 2, (char *)&MenuCursor, 3, 0);
        UiRows[MenuCursor].cells[0] = state;
        if (PlayerControllers[0].action28) {
            SetMenuFrameOptions(0, 1, 1);
            UiRows[MenuCursor].cells[0] = 1;
            if (MenuCursor == 0) {
                if (CourseChoiceTable[1][0].glyph != -1) {
                    SetModelTexturePage(-1);
                    PlaySfx(1);
                    SubmodeState = 3;
                    FadeCountdown = 64;
                    for (i = 0; i < 3; i++) {
                        if (UiRows[i].byte14 == 2) {
                            UiRows[i].byte14 = 7;
                        }
                    }
                } else {
                    PlaySfx(4);
                }
            } else if (MenuCursor == 1) {
                PlaySfx(1);
                SetModelTexturePage(-1);
                SubmodeState = 4;
                for (i = 0; i < 3; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 7;
                    }
                }
                FadeCountdown = 64;
            } else if (MenuCursor == state) {
                PlaySfx(1);
                SetModelTexturePage(-1);
                SubmodeState = 8;
                for (i = 0; i < 3; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 3;
                    }
                }
                FadeCountdown = 64;
                SeqFadeVolume(0, 60);
            }
        } else if (PlayerControllers[0].action2C) {
            PlaySfx(2);
            SetModelTexturePage(-1);
            SubmodeState = 8;
            SetMenuFrameOptions(0, 1, 1);
            for (i = 0; i < 3; i++) {
                if (UiRows[i].byte14 == 2) {
                    UiRows[i].byte14 = 3;
                }
            }
            FadeCountdown = 64;
            SeqFadeVolume(0, 60);
        }
        break;
    case 3:
        FadeCountdown--;
        AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,
                      (64 - FadeCountdown) * 4, 69);
        if (FadeCountdown <= 0) {
            ModeGroup = 1;
            ModeIndex = 63;
            SubmodeState = 0;
        }
        break;
    case 4:
        FadeCountdown--;
        AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,
                      (64 - FadeCountdown) * 4, 69);
        if (FadeCountdown <= 0) {
            InitTimTemplate(&GarageDefaultCarTimBuf);
            UploadCarClut(&GarageDefaultCarTimBuf, 0, 0);
            ModeGroup = 3;
            ModeIndex = 1;
            SubmodeState = 0;
        }
        break;
    case 8:
        FadeCountdown--;
        AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,
                      (64 - FadeCountdown) * 4, 69);
        if (FadeCountdown <= 0) {
            ModeGroup = 1;
            ModeIndex = 3;
            SubmodeState = 0;
            SeqMuteVolume();
        }
        break;
    }
    for (i = 0; i < 3; i++) {
        DrawUiRow(i);
    }
    UpdateScreenFade(0x10043);
}
