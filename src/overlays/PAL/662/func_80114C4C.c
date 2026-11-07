#include "rr/overlay662_progress_controller.h"
#include "rr/course_choice_types.h"

extern RrCourseChoice4 CourseChoiceTable[2][9];
extern s16 MenuCursor;
extern u8 D_800F4368[];
void SetModelTexturePage(s32 mode);
void StepMenuValue(s32 mode, s32 width, char *selection, s32 limit, s32 flags);
void SeqFadeVolume(s32, s32);
void func_80053CB0(void);
void func_8006F5F0(void *);
void UploadCarClut(void *, s32, s32);

void ovl662_func_80114C4C(void)
{
    s32 i;
    s32 state;
    s32 unused[2]; /* never referenced; original frame reserves 8 local bytes */

    ModeFrame++;
    switch ((u16)D_800F4D8A) {
    case 0:
        state = UiRows->byte14;
        if (state != 2) {
            break;
        }
        SetMenuFrameOptions(1, 1, 1);
        UiRows[MenuCursor].cells[0] = 0;
        StepMenuValue(0, 2, (char *)&MenuCursor, 3, 0);
        UiRows[MenuCursor].cells[0] = state;
        if (PlayerControllers[0].a) {
            SetMenuFrameOptions(0, 1, 1);
            UiRows[MenuCursor].cells[0] = 1;
            if (MenuCursor == 0) {
                if (CourseChoiceTable[1][0].glyph != -1) {
                    SetModelTexturePage(-1);
                    PlaySfx(1);
                    D_800F4D8A = 3;
                    D_800AC5D0 = 64;
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
                D_800F4D8A = 4;
                for (i = 0; i < 3; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 7;
                    }
                }
                D_800AC5D0 = 64;
            } else if (MenuCursor == state) {
                PlaySfx(1);
                SetModelTexturePage(-1);
                D_800F4D8A = 8;
                for (i = 0; i < 3; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 3;
                    }
                }
                D_800AC5D0 = 64;
                SeqFadeVolume(0, 60);
            }
        } else if (PlayerControllers[0].b) {
            PlaySfx(2);
            SetModelTexturePage(-1);
            D_800F4D8A = 8;
            SetMenuFrameOptions(0, 1, 1);
            for (i = 0; i < 3; i++) {
                if (UiRows[i].byte14 == 2) {
                    UiRows[i].byte14 = 3;
                }
            }
            D_800AC5D0 = 64;
            SeqFadeVolume(0, 60);
        }
        break;
    case 3:
        D_800AC5D0--;
        AddScreenFadeTiles(((RrFrameOrderingPrefix *)CurrentDrawBuf)->ordering70,
                      (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            ModeGroup = 1;
            D_800F3A26 = 63;
            D_800F4D8A = 0;
        }
        break;
    case 4:
        D_800AC5D0--;
        AddScreenFadeTiles(((RrFrameOrderingPrefix *)CurrentDrawBuf)->ordering70,
                      (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            func_8006F5F0(D_800F4368);
            UploadCarClut(D_800F4368, 0, 0);
            ModeGroup = 3;
            D_800F3A26 = 1;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        D_800AC5D0--;
        AddScreenFadeTiles(((RrFrameOrderingPrefix *)CurrentDrawBuf)->ordering70,
                      (64 - D_800AC5D0) * 4, 69);
        if (D_800AC5D0 <= 0) {
            ModeGroup = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
            func_80053CB0();
        }
        break;
    }
    for (i = 0; i < 3; i++) {
        DrawUiRow(i);
    }
    UpdateScreenFade(0x10043);
}
