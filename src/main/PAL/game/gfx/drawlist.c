/* RODATA 0x800101f4 0xd8 */
#include "types.h"
#include "rr/frame_storage_types.h"
#include "rr/load_cursor.h"
#include "scratchpad.h"
#include "rr_asm.h"
#include "rr/animated_sprite_grid.h"
#include "game/data/cdfile2.h"
#include "game/gfx/gtemodel.h"
#include "game/gfx/light2.h"
#include "game/gfx/skyrot1.h"
#include "game/sound/cdstop.h"
#include "game/effect/spfade.h"
#include "game/data/ldscn5.h"
#include "game/data/cdfile1.h"
#include "rr/sprite_preset.h"
#include "rr/menu_background.h"
#include "rr/memory.h"
#include "game/gfx/gfxotag.h"
#include "game/gfx/model.h"
#include "game/gfx/polygt4.h"
#include "rr/badge_row_types.h"
#include "rr/menu_text_state.h"
#include "rr/course_choices.h"
#include "rr/screen_tiles.h"
#include "game/course/trkintp2.h"
#include "game/camera/viewcam.h"
#include "game/menu/mnrows.h"
#include "game/data/ldscn1.h"
#include "game/ui/uibar.h"
#include "game/ui/uidraw.h"
#include "game/sound/cdplay.h"
#include "game/sound/cdstart.h"

#define G IntroSceneStatePtr
#define H BadgeRows
#define K CardDialogueStatePtr
#define SPAD ((int *)SCRATCHPAD)
#define CURSOR (scratch->cursor)
#define ORDER (order_page->order)


struct Vec4Words16 { int w[4]; };

struct GpuScratchCursorPair { struct GpuPrimPacket20 *cur; struct GpuOrderTablePair *ot; };

union GpuPacketHeader { struct GpuPrimPacket20 *p; struct { char link[3]; unsigned char len; } b; };

struct GpuPrimPacket20 { union GpuPacketHeader h; int w[4]; };

struct GpuOrderTablePair { int x; union GpuPacketHeader h; };

extern void ClearScreenOverlayState();
extern int CdAudioInit();
extern int SetGeomOffset();
extern BadgeRow1C *BadgeRows;
extern short FadeCountdown;
extern char ScreenFadeState;
extern short ModeIndex;
extern short MenuScriptId;
extern short ModeGroup;
extern int DialogueSceneMode;
extern short SubmodeState;
extern short DrawListDefaultVerts;
#include "rr/cd_directory_buffer.h"
extern const Vec3s8 TitleModelZeroAngles;
extern const VectorWords16 TitleModelPosLayer0A;
extern const VectorWords16 TitleModelPosLayer0B;
extern const VectorWords16 TitleModelPosLayer1;
extern char IntroCameraPosKeyframes[];

void StepIntroScene();
void EmitSpriteGrid(u32 intensity,SpriteGridClock *state);

void StepIntroScene(void) {
    int idx;
    int temp_s5;
    register IntroSceneState *g asm("$2"); /* ASM_BARRIER: pin G reload pointer to $v0 */
    register IntroSceneState *g_a0 asm("$4"); /* ASM_BARRIER: pin G reload pointer to $a0 in case 0x2F */
    IntroSceneState *g_v1;
    int temp_a0;
    int temp_a0_10;
    int temp_a0_2;
    int temp_a0_3;
    int temp_a0_4;
    int temp_a0_5;
    int temp_a0_6;
    int temp_a0_7;
    int temp_a0_8;
    int temp_a0_9;
    short *temp_a1;
    short *temp_a1_10;
    short *temp_a1_11;
    short *temp_a1_12;
    short *temp_a1_13;
    short *temp_a1_14;
    short *temp_a1_15;
    short *temp_a1_16;
    short *temp_a1_2;
    short *temp_a1_3;
    short *temp_a1_4;
    short *temp_a1_5;
    short *temp_a1_6;
    short *temp_a1_7;
    short *temp_a1_8;
    short *temp_a1_9;
    int temp_t0;
    int temp_v0;
    int temp_v1;
    int temp_v1_10;
    int temp_v1_2;
    int temp_v1_3;
    int temp_v1_4;
    int temp_v1_5;
    int temp_v1_6;
    int temp_v1_7;
    int temp_v1_8;
    int temp_v1_9;
    int var_a0;
    int var_a0_2;
    int var_a0_3;
    int var_a0_4;
    int var_a0_5;
    int v0;
    struct Vec4Words16 *dv;
    register char *src asm("$2"); /* ASM_BARRIER: pin src to $2 as in original copy sequence */
    int *st;
    int *st_v1;
    register int c80 asm("$3"); /* ASM_BARRIER: pin hoisted 0x80 constant to $v1 in case 0x2F */
    int *sp1, *sp2, *sp3, *sp4;
    int *tabq;
    int *tabr;
    int mid;
    int c_e1000240;
    int c_800080;
    int c_800060;
    int var_a1;
    int var_a1_2;
    int var_a1_3;
    int var_a1_4;
    int var_a1_5;
    int var_a2;
    int var_v0;
    int var_v0_2;
    int var_v0_3;
    int var_v0_4;
    char *temp_s0, *temp_s1, *temp_s2;
    struct GpuPrimPacket20 *temp_t1;
    struct GpuScratchCursorPair *spd, *spd2;
    int col;
    temp_s5 = PollCdAudioStatus();
    if (TitleSceneStage >= 5) {
        if (FadeCountdown != 0) {
            if (FadeCountdown == 4) {
                CdAudioFade(0, 0x40);
            } else if (FadeCountdown >= 0x100) {
                DialogueSceneMode = 0;
                MenuScriptId = 1;
                ModeGroup = 1;
                ModeIndex = 0x23;
                SubmodeState = 0;
            }
            AddScreenFadeTiles(&CurrentDrawBuf->buffer.ordering70.words[0], FadeCountdown, 0x45);
            FadeCountdown = (u16) FadeCountdown + 4;
        } else if (Pad0Buttons & 0x800) {
            FadeCountdown = 4;
        }
    }
    if (TitleSceneStage < 0x36U) {
    switch (TitleSceneStage) {
case 0x0:
    LoadCursor = CdDirectoryBuffer;
    ClearScreenOverlayState();
    ScreenFadeState = 2;
    InitMenuArenas();
    ResetScreenCounters();
    DialogueSceneMode = 4;
    InitBadgeRow(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    TitleSceneStage = 1;
case 0x1:
    if (CdFileReadStart(GetR4FileIndex(0, 0x13), LoadCursor) == 0) {
        return;
    }
    TitleSceneStage = 2;
case 0x2:
    temp_v0 = CdFileBytesLoaded();
    if (temp_v0 == 0) {
        return;
    }
    temp_s1 = LoadCursor + ((int *)LoadCursor)[1];
    temp_s2 = LoadCursor + ((int *)LoadCursor)[2];
    temp_s0 = LoadCursor + ((int *)LoadCursor)[4];
    LoadTimImage(LoadCursor + ((int *)LoadCursor)[3]);
    LoadTimImage(temp_s0);
    RegisterModelBank((RroModelBank *)temp_s1, 4);
    SkyCommandListPtr = temp_s2;
    RelocateOffsetTable(temp_s2);
    TitleSceneStage = 3;
    LoadCursor += temp_v0;
case 0x3:
    if (LoadMenuCarModelsStep() == 0) {
        return;
    }
    CdAudioInit();
    CdAudioSelectTrack(0, 0);
    CdAudioBeginPlay(0);
    InitIntroSceneLights();
    TitleSceneStage = 4;
case 0x4:
    if (temp_s5 < 0) {
        return;
    }
    TitleSceneStage = 5;
    G->s48 = 0x80;
    G->s9C = 0x1000;
    G->s4A = 0;
    G->p40 = &DrawListDefaultVerts;
    G->s46 = 0;
    G->s44 = 0;
case 0x5:
    if (G->s46 < 0xB4) {
        break;
    }
    if (G->s9C > 0) {
        G->s9C = (s16) ((u16) G->s9C - 0x100);
    }
    if (((u16) G->s46 - 0xA0) < 0xAAU) {
        sp1 = (int *)SCRATCHPAD;
        sp1[2] = sp1[2] + 3;
    }
    temp_a0 = G->s46;
    if (temp_a0 == 0xB0) {
        G->s76 = 4;
    } else if (temp_a0 == 0xA0) {
        G->s8A = 0x80;
        G->s8C = 0x80;
        G->s8E = 0x80;
    } else if (temp_a0 == 0x15E) {
        H->half02 = 1;
    } else if (temp_a0 == 0x129) {
        G->s8A = -0x80;
        G->s8C = -0x80;
        G->s8E = -0x80;
    } else if (temp_a0 == 0x175) {
        G->s74 = -4;
        G->s76 = -4;
    }
    temp_a1 = G->p40;
    if (G->s46 < *temp_a1) {
        goto block_197;
    }
    G->p40 = (s16 *) (temp_a1 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 6;
    G->s44 = 0;
case 0x6:
    if (G->s44 < 0x20) {
        goto block_197;
    }
    PsyqMemset((SCRATCHPAD + 0x18), 0, 0x10);
    src = IntroCameraPosKeyframes;
    st = &TitleSceneStage;
    sp2 = (int *)(SCRATCHPAD + 0x8);
    RR_COPY16_T(sp2, src, 16);
    g = G;
    *st = 7;
    g->s56 = 0x100;
    g->uA2 = 3U;
    g->s8A = 0x80;
    g->s48 = 0;
    g->s5E = 0;
    g->s66 = 0;
    g->s68 = 0;
    g->s6A = 0;
    g->s74 = 4;
    g->s44 = 0;
case 0x7:
    temp_v1 = G->s44;
    if (temp_v1 < 0x38) {
        if (temp_v1 == 8) {
            G->s82 = 0x200;
        } else if (temp_v1 == 0xC) {
            G->s76 = 4;
        } else if (temp_v1 == 0x18) {
            G->s78 = 4;
        }
        goto block_199;
    }
    G->s44 = 0;
    TitleSceneStage = 8;
case 0x8:
    temp_a1_2 = G->p40;
    if (G->s46 < *temp_a1_2) {
        goto block_199;
    }
    G->p40 = (s16 *) (temp_a1_2 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 9;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->uA2 = 0xCU;
    G->s9E = 2;
    G->sA0 = 0x1000;
    G->s8A = -0x80;
    G->s8C = 0x80;
    G->s44 = 0;
case 0x9:
    if (G->s44 < 0x20) {
        goto block_199;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->objectYawAA[1] = 0;
    G->objectYawAA[0] = 0;
    G->s74 = 4;
    G->s44 = 0;
    TitleSceneStage = 0xA;
case 0xA:
    temp_v1_2 = G->s44;
    if (temp_v1_2 < 0x38) {
        if (temp_v1_2 == 8) {
            G->s82 = 0x200;
        } else if (temp_v1_2 == 0xC) {
            G->s76 = 4;
        } else if (temp_v1_2 == 0x18) {
            G->s78 = 4;
        }
        goto block_201;
    }
    G->s44 = 0;
    TitleSceneStage = 0xB;
case 0xB:
    temp_a1_3 = G->p40;
    if (G->s46 < *temp_a1_3) {
        goto block_201;
    }
    G->p40 = (s16 *) (temp_a1_3 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0xC;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->uA2 = 0xF0U;
    G->s9E = 3;
    G->sA0 = 0x1000;
    G->s8C = -0x80;
    G->s8E = 0x80;
    G->s44 = 0;
case 0xC:
    if (G->s44 < 0x20) {
        goto block_201;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->objectYawAA[3] = 0;
    G->objectYawAA[2] = 0;
    G->s74 = 4;
    G->s44 = 0;
    TitleSceneStage = 0xD;
case 0xD:
    temp_v1_3 = G->s44;
    if (temp_v1_3 < 0x50) {
        if (temp_v1_3 == 8) {
            G->s82 = 0x200;
        } else if (temp_v1_3 == 0xC) {
            G->s76 = 4;
        } else if (temp_v1_3 == 0x18) {
            G->s78 = 4;
        } else if (temp_v1_3 == 0x24) {
            G->s7A = 4;
        } else if (temp_v1_3 == 0x30) {
            G->s7C = 4;
        }
        goto block_203;
    }
    G->s44 = 0;
    TitleSceneStage = 0xE;
case 0xE:
    temp_a1_4 = G->p40;
    if (G->s46 < *temp_a1_4) {
        goto block_203;
    }
    G->p40 = (s16 *) (temp_a1_4 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0xF;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s7A = -4;
    G->s7C = -4;
    G->s82 = -8;
    G->s8A = 0x80;
    G->s8C = 0x80;
    G->s9E = 4;
    G->uA2 = 0U;
    G->sA0 = 0x1000;
    G->s44 = 0;
case 0xF:
    if (G->s44 < 0x20) {
        goto block_203;
    }
    if (G->sA4 != 0) {
        goto block_203;
    }
    G->s56 = 0x100;
    G->s5E = 0;
    G->s4A = 0;
    G->s48 = 0;
    G->s70 = 0;
    G->s6E = 0;
    G->s6C = 0;
    G->s74 = 4;
    G->s44 = 0;
    TitleSceneStage = 0x10;
case 0x10:
    temp_a0_2 = G->s44;
    if (temp_a0_2 < 0x38) {
        if (temp_a0_2 == 8) {
            G->s82 = 0x200;
        } else if (temp_a0_2 == 0xC) {
            G->s90 = 0x40;
            G->s92 = 0x40;
            G->s94 = 0x40;
        } else if (temp_a0_2 == 0x18) {
            G->s76 = 4;
        } else if (temp_a0_2 == 0x4C) {
            G->s94 = 0;
            G->s92 = 0;
            G->s90 = 0;
        }
        goto block_206;
    }
    G->sA6 = 0xFFF;
    G->s44 = 0;
    TitleSceneStage = 0x11;
case 0x11:
    temp_a1_5 = G->p40;
    if (G->s46 < *temp_a1_5) {
        goto block_206;
    }
    G->p40 = (s16 *) (temp_a1_5 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x12;
case 0x12:
    G->sA6 = 0;
    TitleSceneStage = 0x13;
    G->uA2 = 1;
    G->s44 = 0;
    goto block_206;
case 0x13:
    temp_a1_6 = G->p40;
    if (G->s46 < *temp_a1_6) {
        goto block_206;
    }
    G->p40 = (s16 *) (temp_a1_6 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x14;
case 0x14:
    TitleSceneStage = 0x15;
    G->sA6 = 1;
    G->uA2 = 2;
    G->s44 = 0;
    goto block_206;
case 0x15:
    temp_a1_7 = G->p40;
    if (G->s46 < *temp_a1_7) {
        goto block_206;
    }
    G->p40 = (s16 *) (temp_a1_7 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x16;
case 0x16:
    G->sA6 = 0;
    TitleSceneStage = 0x17;
    G->uA2 = 0U;
    G->s44 = 0;
    goto block_206;
case 0x17:
    if (G->sA4 != 0) {
        goto block_206;
    }
    G->s76 = -4;
    G->s8A = -0x40;
    G->s8C = -0x40;
    G->s8E = -0x40;
    G->s90 = -0x40;
    G->s92 = -0x40;
    G->s94 = -0x40;
    G->s44 = 0;
    TitleSceneStage = 0x18;
case 0x18:
    if (G->s44 < 0x20) {
        goto block_206;
    }
    G->s44 = 0;
    TitleSceneStage = 0x1B;
case 0x1B:
    temp_a1_8 = G->p40;
    if (G->s46 < *temp_a1_8) {
        goto block_206;
    }
    G->p40 = (s16 *) (temp_a1_8 + 1);
    K->b4 = K->b4 + 1;
    st = &TitleSceneStage;
    g = G;
    *st = 0x19;
    g->s56 = 0x100;
    g->s58 = 0x100;
    g->s5A = 0x100;
    g->s90 = 0x40;
    g->s92 = 0x40;
    g->s94 = 0x40;
    g->s54 = 0;
    g->s52 = 0;
    g->s50 = 0;
    g->s4E = 0;
    g->s4C = 0;
    g->s4A = 0;
    g->s48 = 0;
    g->s5E = 0;
    g->s60 = 0;
    g->s62 = 0;
    g->s74 = 8;
    g->s44 = 0;
case 0x19:
    if (G->s44 == 0x20) {
        G->s94 = 0;
        G->s92 = 0;
        G->s90 = 0;
    }
    temp_a1_9 = G->p40;
    if (G->s46 < *temp_a1_9) {
        goto block_210;
    }
    G->p40 = (s16 *) (temp_a1_9 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x1C;
    G->s90 = 0x40;
    G->s44 = 0;
case 0x1C:
    temp_v1_4 = G->s44;
    if (temp_v1_4 < 0x44) {
        if (temp_v1_4 == 0x10) {
            G->s76 = 4;
        } else if (temp_v1_4 == 0x18) {
            G->s82 = 0x200;
        } else if (temp_v1_4 == 0x24) {
            G->s78 = 4;
        }
        goto block_210;
    }
    G->s44 = 0;
    TitleSceneStage = 0x1D;
case 0x1D:
    temp_a1_10 = G->p40;
    if (G->s46 < *temp_a1_10) {
        goto block_210;
    }
    G->p40 = (s16 *) (temp_a1_10 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x1E;
    G->s92 = 0x40;
    G->s44 = 0;
case 0x1E:
    temp_v1_5 = G->s44;
    if (temp_v1_5 < 0x44) {
        if (temp_v1_5 == 0x10) {
            G->s7A = 4;
        } else if (temp_v1_5 == 0x18) {
            G->s84 = 0x200;
        } else if (temp_v1_5 == 0x24) {
            G->s7C = 4;
        }
        goto block_210;
    }
    G->s44 = 0;
    TitleSceneStage = 0x1F;
case 0x1F:
    temp_a1_11 = G->p40;
    if (G->s46 < *temp_a1_11) {
        goto block_210;
    }
    G->p40 = (s16 *) (temp_a1_11 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x20;
    G->s94 = 0x40;
    G->s44 = 0;
case 0x20:
    temp_v1_6 = G->s44;
    if (temp_v1_6 < 0x44) {
        if (temp_v1_6 == 0x10) {
            G->s7E = 4;
        } else if (temp_v1_6 == 0x18) {
            G->s86 = 0x200;
        } else if (temp_v1_6 == 0x24) {
            G->s80 = 4;
        }
        goto block_210;
    }
    G->s44 = 0;
    TitleSceneStage = 0x21;
case 0x21:
    temp_a1_12 = G->p40;
    if (G->s46 < *temp_a1_12) {
        goto block_210;
    }
    G->p40 = (s16 *) (temp_a1_12 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x22;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s7A = -4;
    G->s7C = -4;
    G->s7E = -4;
    G->s80 = -4;
    G->s82 = -8;
    G->s84 = -8;
    G->s86 = -8;
    G->s44 = 0;
case 0x22:
    if (G->s44 < 0x20) {
        goto block_210;
    }
    G->s9E = 5;
    G->sA0 = 0x1000;
    G->s44 = 0;
    TitleSceneStage = 0x23;
case 0x23:
    if (G->s44 < 0x20) {
        goto block_210;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->s74 = 4;
    G->s44 = 0;
    TitleSceneStage = 0x24;
case 0x24:
    temp_v1_7 = G->s44;
    if (temp_v1_7 < 0x44) {
        if (temp_v1_7 == 0x10) {
            G->s76 = 4;
        } else if (temp_v1_7 == 0x18) {
            G->s82 = 0x200;
        } else if (temp_v1_7 == 0x24) {
            G->s78 = 4;
        }
        goto block_212;
    }
    G->s44 = 0;
    TitleSceneStage = 0x25;
case 0x25:
    temp_a1_13 = G->p40;
    if (G->s46 < *temp_a1_13) {
        goto block_212;
    }
    G->p40 = (s16 *) (temp_a1_13 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x26;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->s90 = -0x80;
    G->s44 = 0;
case 0x26:
    if (G->s44 < 0x20) {
        goto block_212;
    }
    G->s9E = 6;
    G->sA0 = 0x1000;
    G->s44 = 0;
    TitleSceneStage = 0x28;
case 0x28:
    if (G->s44 < 0x20) {
        goto block_212;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->s74 = 4;
    G->s44 = 0;
    TitleSceneStage = 0x29;
case 0x29:
    temp_v1_8 = G->s44;
    if (temp_v1_8 < 0x44) {
        if (temp_v1_8 == 0x10) {
            G->s76 = 4;
        } else if (temp_v1_8 == 0x18) {
            G->s82 = 0x200;
        } else if (temp_v1_8 == 0x24) {
            G->s78 = 4;
        }
        goto block_214;
    }
    G->s44 = 0;
    TitleSceneStage = 0x2A;
case 0x2A:
    temp_a1_14 = G->p40;
    if (G->s46 < *temp_a1_14) {
        goto block_214;
    }
    G->p40 = (s16 *) (temp_a1_14 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x2B;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->s92 = -0x80;
    G->s44 = 0;
case 0x2B:
    if (G->s44 < 0x20) {
        goto block_214;
    }
    G->s9E = 7;
    G->sA0 = 0x1000;
    G->s44 = 0;
    TitleSceneStage = 0x2C;
case 0x2C:
    if (G->s44 < 0x20) {
        goto block_216;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->s74 = 4;
    G->s44 = 0;
    TitleSceneStage = 0x2D;
case 0x2D:
    temp_v1_9 = G->s44;
    if (temp_v1_9 < 0x44) {
        if (temp_v1_9 == 0x10) {
            G->s76 = 4;
        } else if (temp_v1_9 == 0x18) {
            G->s82 = 0x200;
        } else if (temp_v1_9 == 0x24) {
            G->s78 = 4;
        }
        goto block_216;
    }
    G->s44 = 0;
    TitleSceneStage = 0x2E;
case 0x2E:
    temp_a1_15 = G->p40;
    if (G->s46 < *temp_a1_15) {
        goto block_216;
    }
    G->p40 = (s16 *) (temp_a1_15 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x2F;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->s94 = -0x80;
    G->s44 = 0;
case 0x2F:
    if (G->s44 < 0x20) {
        goto block_216;
    }
    c80 = 0x80;
    G->s56 = 0x100;
    G->s5E = 0x1000;
    G->s76 = 4;
    __asm__ (""); /* ASM_BARRIER: keep src load after s76 store */
    src = IntroCameraPosKeyframes;
    G->s48 = c80;
    G->s4A = 0;
    G->s72 = 0;
    G->s96 = c80;
    __asm__ ("" ::: "memory"); /* ASM_BARRIER: force G reload after s96 store */
    g_a0 = G;
    __asm__ ("" : "+r"(g_a0) :: "$8", "$9"); /* ASM_BARRIER: emit G reload before state lui and reserve $t0-$t1 for sp2 in $t5 */
    st_v1 = &TitleSceneStage;
    sp2 = (int *)(SCRATCHPAD + 0x8);
    RR_COPY16_T(sp2, src, 128);
    *st_v1 = 0x30;
    g_a0->s44 = 0;
case 0x30:
    if (G->s44 < 0x20) {
        goto block_219;
    }
    G->s44 = 0;
    TitleSceneStage = 0x31;
case 0x31:
    temp_a1_16 = G->p40;
    if (G->s46 < *temp_a1_16) {
        goto block_219;
    }
    G->p40 = (s16 *) (temp_a1_16 + 1);
    K->b4 = K->b4 + 1;
    TitleSceneStage = 0x32;
    G->s74 = -4;
    G->s76 = -4;
    G->s82 = -8;
    G->s96 = -0x80;
    G->s44 = 0;
case 0x32:
    if (G->s44 < 0x20) {
        goto block_219;
    }
    sp4 = (int *)SCRATCHPAD;
    sp4[6] = 0x400;
    sp4[7] = 0;
    sp4[8] = 0;
    __asm__ volatile("" : "=r"(temp_a0) :: "memory"); /* ASM_BARRIER: clobber $a0 so %hi(IntroSceneStatePtr) stays in $a1 across sp4 stores */
    src = IntroCameraPosKeyframes;
    g_v1 = G;
    __asm__ ("" : "+r"(src), "+r"(g_v1) :: "$8", "$9"); /* ASM_BARRIER: emit src and G reload before state lui and reserve $t0-$t1 for sp2 in $t5 */
    st = &TitleSceneStage;
    sp2 = (int *)(SCRATCHPAD + 0x8);
    RR_COPY16_T(sp2, src, 144);
    *st = 0x33;
    g_v1->s48 = 0;
    g_v1->s74 = 8;
    g_v1->s98 = 8;
    g_v1->s44 = 0;
case 0x33:
    if (G->s44 < 0x20) {
        goto block_222;
    }
    G->s44 = 0;
    TitleSceneStage = 0x34;
case 0x34:
    if (G->s46 >= *G->p40) {
        FadeCountdown = 4;
        TitleSceneStage = 0x35;
    }
    goto block_222;
block_197:
    EmitTitleBeamFades(G->s48, G->s9C);
    EmitPresetSprite(G->s4A, 0);
    break;
block_199:
    EmitPresetSprite(G->s48, 1);
    EmitPresetSprite(G->s48, 2);
    EmitPresetSprite(G->s4A, 3);
    EmitPresetSprite(G->s4C, 4);
    EmitFadeTile(G->s56, G->s5E, 0x6C0018, 0x78);
    break;
block_201:
    EmitPresetSprite(G->s48, 5);
    EmitPresetSprite(G->s4A, 6);
    EmitPresetSprite(G->s4C, 7);
    EmitFadeTile(G->s56, G->s5E, 0x6C0018, 0x78);
    break;
block_203:
    EmitPresetSprite(G->s48, 8);
    EmitPresetSprite(G->s4A, 9);
    EmitPresetSprite(G->s4C, 0xA);
    EmitPresetSprite(G->s4E, 0xB);
    EmitPresetSprite(G->s50, 0xC);
    EmitFadeTile(G->s56, G->s5E, 0x6C0018, 0x78);
    break;
block_206:
    temp_a0_3 = G->sA6;
    if (temp_a0_3 != 0) {
        EmitTitleMaskSprites(temp_a0_3, G->s44);
    }
    EmitPresetSprite(G->s48, 0xD);
    EmitPresetSprite(G->s48, 0xE);
    EmitFadeTile(G->s56, G->s5E, 0x680010, 0x120);
    EmitPresetSprite(G->s4A, 0xF);
    EmitPresetSprite(G->s4A, 0x10);
    EmitPresetSprite(G->s4A, 0x11);
    break;
block_210:
    EmitPresetSprite(G->s48, 0xF);
    EmitPresetSprite(G->s48, 0x10);
    EmitPresetSprite(G->s48, 0x11);
    EmitPresetSprite(G->s4A, 0x12);
    EmitPresetSprite(G->s4C, 0x13);
    EmitPresetSprite(G->s4E, 0x14);
    EmitPresetSprite(G->s50, 0x15);
    EmitPresetSprite(G->s52, 0x16);
    EmitPresetSprite(G->s54, 0x17);
    EmitFadeTile(G->s56, G->s5E, 0xB0001C, 0x28);
    EmitFadeTile(G->s58, G->s60, 0xB00070, 0x28);
    EmitFadeTile(G->s5A, G->s62, 0xB000E0, 0x28);
    goto block_218;
block_212:
    EmitPresetSprite(G->s48, 0x18);
    EmitPresetSprite(G->s48, 0x19);
    EmitPresetSprite(G->s4A, 0x1A);
    EmitPresetSprite(G->s4C, 0x1B);
    EmitFadeTile(G->s56, G->s5E, 0xE0008C, 0x28);
    goto block_218;
block_214:
    EmitPresetSprite(G->s48, 0x1C);
    EmitPresetSprite(G->s4A, 0x1D);
    EmitPresetSprite(G->s4C, 0x1E);
    EmitFadeTile(G->s56, G->s5E, 0xE0008C, 0x28);
    goto block_218;
block_216:
    EmitPresetSprite(G->s48, 0x1F);
    EmitPresetSprite(G->s4A, 0x20);
    EmitPresetSprite(G->s4C, 0x21);
    EmitFadeTile(G->s56, G->s5E, 0xE0008C, 0x28);
block_218:
    EmitPresetSprite(0x80, 0xD);
    EmitPresetSprite(0x80, 0xE);
    EmitFadeTile(0x100, 0x1000, 0x680010, 0x120);
    break;
block_219:
    EmitPresetSprite(G->s48, 0xD);
    EmitPresetSprite(G->s48, 0xE);
    EmitPresetSprite(G->s4A, 0x22);
    EmitPresetSprite(G->s4A, 0x23);
    EmitFadeTile(G->s56, G->s5E, 0x680010, 0x120);
    break;
case 0x35:
block_222:
    EmitPresetSprite(G->s48, 0x24);
    break;
case 0x1A:
case 0x27:
    break;
    }
    }
    G->s44 = (s16) ((u16) G->s44 + 1);
    G->s46 = (s16) ((u16) G->s46 + 1);
    spd = (struct GpuScratchCursorPair *)SCRATCHPAD;
    spd->cur->w[0] = 0xE1000229;
    spd->cur->h.p = spd->ot->h.p;
    spd->cur->h.b.len = 1;
    spd->ot->h.p = spd->cur;
    spd->ot->h.b.len = 0;
    spd->cur = (struct GpuPrimPacket20 *)((char *)spd->cur + 8);
    RampValueToLimit(&G->s48, &G->s74, 0x80);
    RampValueToLimit(&G->s4A, &G->s76, 0x80);
    RampValueToLimit(&G->s4C, &G->s78, 0x80);
    RampValueToLimit(&G->s4E, &G->s7A, 0x80);
    RampValueToLimit(&G->s50, &G->s7C, 0x80);
    RampValueToLimit(&G->s52, &G->s7E, 0x80);
    RampValueToLimit(&G->s54, &G->s80, 0x80);
    RampValueToLimit(&G->s9A, &G->s98, 0x100);
    RampValuePairTo4096(&G->s56, &G->s5E, &G->s82);
    RampValuePairTo4096(&G->s58, &G->s60, &G->s84);
    RampValuePairTo4096(&G->s5A, &G->s62, &G->s86);
    RampValuePairTo4096(&G->s5C, &G->s64, &G->s88);
    RampValueTo4096(&G->s66, &G->s8A);
    RampValueTo4096(&G->s68, &G->s8C);
    RampValueTo4096(&G->s6A, &G->s8E);
    RampValueTo4096(&G->s6C, &G->s90);
    RampValueTo4096(&G->s6E, &G->s92);
    RampValueTo4096(&G->s70, &G->s94);
    RampValueTo4096(&G->s72, &G->s96);
    if (G->sA0 != 0) {
        G->sA0 -= 0x80;
        RR_GTE_MTC2_IR0((unsigned short)G->sA0);
        idx = G->s9E;
        RR_GTE_LWC2_IR((int *)IntroCameraPosKeyframes + idx * 4);
        RR_GTE_SETFC_P((int *)IntroCameraPosKeyframes + (idx - 1) * 4);
        RR_GTE_INTPL();
        RR_GTE_STORE_IR((int *)(SCRATCHPAD + 0x8));
    }
    BuildScaledCameraMatrices();
    SetupViewport(6);
    SetGeomOffset(0xA0, 0xC0);
    temp_a0_4 = G->s66;
    G->sA4 = 0;
    if (temp_a0_4 != 0) {
        SetLightColorsFromAngle(temp_a0_4);
        G->sA4 = (s16) ((u16) G->sA4 | DrawRotatingTitleModel(0, G->uA2 & 1));
        G->sA4 = (s16) ((u16) G->sA4 | DrawRotatingTitleModel(1, G->uA2 & 2));
    }
    temp_a0_5 = G->s68;
    if (temp_a0_5 != 0) {
        SetLightColorsFromAngle(temp_a0_5);
        G->sA4 = (s16) ((u16) G->sA4 | DrawRotatingTitleModel(2, G->uA2 & 4));
        G->sA4 = (s16) ((u16) G->sA4 | DrawRotatingTitleModel(3, G->uA2 & 8));
    }
    temp_a0_6 = G->s6A;
    if (temp_a0_6 != 0) {
        SetLightColorsFromAngle(temp_a0_6);
        G->sA4 = (s16) ((u16) G->sA4 | DrawRotatingTitleModel(4, G->uA2 & 0x10));
        G->sA4 = (s16) ((u16) G->sA4 | DrawRotatingTitleModel(5, G->uA2 & 0x20));
        G->sA4 = (s16) ((u16) G->sA4 | DrawRotatingTitleModel(6, G->uA2 & 0x40));
        G->sA4 = (s16) ((u16) G->sA4 | DrawRotatingTitleModel(7, G->uA2 & 0x80));
    }
    temp_a0_7 = G->s6C;
    if (temp_a0_7 != 0) {
        SetLightColorsFromAngle(temp_a0_7);
        DrawTransformedTitleModel(1, &TitleModelZeroAngles, &TitleModelPosLayer0A);
    }
    temp_a0_8 = G->s6E;
    if (temp_a0_8 != 0) {
        SetLightColorsFromAngle(temp_a0_8);
        DrawTransformedTitleModel(1, &TitleModelZeroAngles, &TitleModelPosLayer0B);
    }
    temp_a0_9 = G->s70;
    if (temp_a0_9 != 0) {
        SetLightColorsFromAngle(temp_a0_9);
        DrawTransformedTitleModel(2, &TitleModelZeroAngles, &TitleModelPosLayer1);
    }
    temp_a0_10 = G->s72;
    if (temp_a0_10 != 0) {
        SetLightColorsFromAngle(temp_a0_10);
        DrawRotatingSky();
    }
    c80 = G->s9A;
    if (c80 != 0) {
        temp_t1 = spd->cur;
        temp_t0 = 0x100 - c80;
        temp_t0 = (temp_t0 < 0 ? 0 : temp_t0) > 0x100 ? 0x100 : (temp_t0 < 0 ? 0 : temp_t0);
        c_800060 = 0x800060;
        c_800080 = 0x800080;
        c_e1000240 = 0xE1000240;
        c80 = temp_t0 << 0x10;
        mid = (temp_t0 << 8) | 0x62000000;
        c80 |= mid;
        c80 |= temp_t0;
        temp_t1->w[1] = c80;
        spd2 = (struct GpuScratchCursorPair *)SCRATCHPAD;
        temp_t1->w[2] = c_800060;
        temp_t1->w[3] = c_800080;
        temp_t1->w[0] = c_e1000240;
        temp_t1->h.p = spd2->ot->h.p;
        temp_t1->h.b.len = 4;
        spd2->ot->h.p = temp_t1;
        spd2->ot->h.b.len = 0;
        temp_t1++;
        spd2->cur = temp_t1;
        DrawMenuGridMesh(0, &spd2->ot->h, 1);
    }
    H->half02 = 1;
    UpdateBadgeRowAnim(0);
    K->b4 = 0U;
    UpdateDialogue(H->x04 + 0xA, H->y06 + 0x28, CardDialogueStatePtr);
    UpdateScreenFade(0x1001C);
}

void EmitSpriteGrid(u32 intensity, SpriteGridClock *state) {
    u32 color = intensity;
    u32 hi, middle;
    s32 row, x, phase, index;
    GpuScratchPointers *scratch;
    struct GpuPrimPacket20 *packet;

    hi = color << 16;
    middle = color << 8;
    middle |= 0x66000000U;
    hi |= middle;
    color |= hi;

    packet = *(struct GpuPrimPacket20 **)SCRATCHPAD;
    state->tick0E++;
    phase = state->tick0E;
    for (row = 0; row < 144; row += 48) {
        for (x = 0; x < 130; x += 26) {
            index = (phase / 4) & 15;
            if (index == 15) index = 0;
            packet->w[1] = ((row + 0xB0) << 16) | (x + 172);
            packet->w[0] = color;
            packet->w[3] = 0x180010;
            packet->w[2] = index * 16 + 0x2F29A400U;
            phase += 5;
            *(u32 *)packet = *((GpuScratchPointers *)SCRATCHPAD)->order;
            packet->h.b.len = 4;
            *((GpuScratchPointers *)SCRATCHPAD)->order = packet;
            ((u8 *)((GpuScratchPointers *)SCRATCHPAD)->order)[3] = 0;
            packet++;
        }
    }
    scratch = (GpuScratchPointers *)SCRATCHPAD;
    scratch->cursor = (u8 *)packet;
    ((u32 *)packet)[1] = 0xE100022A;
    *(u32 *)scratch->cursor = *scratch->order;
    scratch->cursor[3] = 1;
    *scratch->order = scratch->cursor;
    ((u8 *)scratch->order)[3] = 0;
    scratch->cursor += sizeof(GpuSingleCommandPacket);
}
