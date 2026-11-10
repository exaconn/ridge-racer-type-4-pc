#include "game/car/carstat.h"
#include "game/data/cdfile2.h"
#include "rr/frame_environment.h"
#include "rr/load_cursor.h"
#include "rr/frame_loop.h"
#include "rr/save_progress.h"
#include "game/car/car.h"
#include "game/car/carstat.h"
#include "game/data/cardini.h"
#include "game/data/cdfile1.h"
#include "game/data/trkld8.h"
#include "game/gfx/fadespr.h"
#include "game/gfx/model.h"
#include "game/menu/menu8.h"
#include "game/race/racesamp.h"
#include "game/race/ranking.h"
#include "psyq/libgpu/gpuenv.h"
#include "psyq/libsio/siostart.h"
#include "psyq/libspu/sinit2.h"
#include "game/data/ldscn1.h"
#include "game/menu/menusave.h"
#include "game/system/frmenv.h"
#include "rr/audio_cd_controller.h"
#include "rr/library.h"
#include "rr/driver_control.h"
#include "rr/bios_devices.h"
#include "game/data/selection.h"
#include "rr/string_tail_copy.h"
#include "game/menu/mnrows.h"

extern void CdAudioInit(void);
extern int InitGeom();

#include "rr/cd_directory_buffer.h"
extern char StrTotalInitLog[];
extern char DefaultPlayerName[];
extern char TeamNameDigRacingTeam[];
extern char TeamAbbrevDRT[];
extern char TeamNameMicroMouseMappy[];
extern char TeamAbbrevMicroMouseMappy[];
extern char StrPacRacingClub[];
extern char TeamAbbrevPRC[];
extern char TeamNameRacingTeamSolvalou[];
extern char TeamAbbrevRTS[];
extern char TeamNameAssoluto[];
extern char TeamNameLizard[];
extern char StrTerrazi[];
extern char StrAgeSolo[];
extern RaceVehicle PlayerCars[2];
extern int MirrorRenderFlag;
extern signed char SelectionState[];
extern short LinkHostFlag;
extern char SaveDataWork[];
extern char D_800F39D8[];
extern char CustomEmblemBuffers[];
extern signed char SaveSlotSelections[];
extern int TeamStringPtrTable[];
extern int RivalTeamNamePtrTable[];
extern int NumLaps;
extern int D_800F4C24;
extern int D_800F4C2C;
extern int DialogueSceneMode;
#include "rr/menu_arena_base.h"
extern short GameModeType;
extern char *CustomEmblemTimPtr;
extern void SetGeomOffset(s32,s32);
extern void SetGeomScreen(s32);

void SetupDrawDispEnvs(s32 r,s32 g,s16 b,u32 flags);

void SystemInit(void)
{
    int i;
    signed char *r;
    int k;
    int m;

    DebugPrintStub(StrTotalInitLog);
    CdInit();
    InitCdFileDirectory();
    SsInit();
    SoundSystemInit();
    CdAudioInit();
    ResetGraph(0);
    SetGraphDebug(0);
    SetDispMask(0);
    InitGeom();
    CardSystemInit(1);
    InitGamePads();
    InitOptionsDefaults();
    ApplyOptionsPadButtonMaps();
    AddSIO();
    _sio_control(1, 1, 0);
    SetupRenderer(5);
    VSyncThreshold = 0x80;
    D_800F4C24 = 0;
    D_800F4C2C = 0;
    DrawSubOtFlag = 0;
    NumLaps = 2;
    MirrorRenderFlag = 0;
    PsyqBzero(&SaveProgress, 0x7C);
    PsyqBzero(SelectionRows, 0x20);
    PsyqBzero(SelectionState, 0x2C);
    SelectionState[0x17] = -1;
    PsyqBzero(RankEntries, 8 * sizeof(*RankEntries));
    i = 7;
    do {
        RankTable[i] = &RankEntries[i];
        i--;
    } while (i >= 0);
    PsyqBzero(&PlayerCars[0], 0x320);
    PsyqBzero(&PlayerCars[1], 0x320);
    UnlockNoticeMask = 0;
    for (i = 0; i < 4; i++) {
        SaveProgress.history6C[i].selection = 5;
        SaveProgress.history6C[i].variant = 5;
        SaveProgress.history6C[i].parameter = 0;
    }
    LinkHostFlag = -1;
    CopyStringClearTail(&SaveProgress, DefaultPlayerName);
    CopyStringClearTail(D_800F39D8, &SaveProgress);
    CopyStringClearTail(D_800F39D8 + 0xC, &SaveProgress);
    CopyStringClearTail(D_800F39D8 + 0x18, &SaveProgress);
    CopyStringClearTail(D_800F39D8 + 0x24, &SaveProgress);
    TeamStringPtrTable[0] = TeamNameDigRacingTeam;
    TeamStringPtrTable[4] = TeamAbbrevDRT;
    TeamStringPtrTable[1] = TeamNameMicroMouseMappy;
    TeamStringPtrTable[5] = TeamAbbrevMicroMouseMappy;
    TeamStringPtrTable[2] = StrPacRacingClub;
    TeamStringPtrTable[6] = TeamAbbrevPRC;
    TeamStringPtrTable[3] = TeamNameRacingTeamSolvalou;
    TeamStringPtrTable[7] = TeamAbbrevRTS;
    RivalTeamNamePtrTable[0] = TeamNameAssoluto;
    RivalTeamNamePtrTable[1] = TeamNameLizard;
    RivalTeamNamePtrTable[2] = StrTerrazi;
    RivalTeamNamePtrTable[3] = StrAgeSolo;
    LoadCursor = CdDirectoryBuffer;
    MenuArenaBase = CdDirectoryBuffer;
    InitMenuArenas();
    k = 0xAF000;
    m = -1;
    i = 2;
    DialogueSceneMode = 0;
    GameModeType = 0;
    LoadDestCursor = CdDirectoryBuffer + k;
    r = SaveSlotSelections + i * 0x2C;
    do {
        r[0x17] = m;
        i--;
        r -= 0x2C;
    } while (i >= 0);
    InitDefaultTimeRecords();
    InitCourseChoices();
    InitTimTemplate(CustomEmblemBuffers);
    InitTimTemplate(CustomEmblemBuffers + 0x840);
    InitTimTemplate(SaveDataWork);
    CustomEmblemTimPtr = CustomEmblemBuffers;
    ResetRankEntries();
    InitCarTable(1);
    ResetGraph(3);
}

void InitFrameEnvStub(s32 bank) {
}

void SetupDrawDispEnvs(s32 r,s32 g,s16 b,u32 flags) {
    /* ASM_BARRIER: keep flags in S0 until the arena replaces their dead value. */
    register u32 input asm("$16")=flags;
    /* ASM_BARRIER: retain red in the original saved register across environment helpers. */
    register s32 red asm("$23")=r;
    s32 green;
    u32 temp;
    /* ASM_BARRIER: retain interlace independently of height and offset. */
    register s32 interlace asm("$19");
    u32 height;
    u32 other_y;
    s32 width;
    /* ASM_BARRIER: retain the rgb24-mode bit across environment helpers. */
    register u32 rgb24 asm("$21");
    /* ASM_BARRIER: preserve flags until the background-fill loop value is extracted. */
    register u32 saved_flags asm("$22");
    /* ASM_BARRIER: reuse S0 for the arena after the input flags have been consumed. */
    register FrameStorage *env asm("$16");
    GpuRectangleFields window;
    s32 i;
    u16 *settings;
    s32 one;
    /* ASM_BARRIER: keep the loop rgb24-mode value in A3 after the helper calls. */
    register u32 loopRgb24 asm("$7");
    /* ASM_BARRIER: keep the loop background-fill value in T0 after the helper calls. */
    register u32 background asm("$8");
    s32 center_x;
    u32 center_y;
    /* ASM_BARRIER: capture red and flags before assigning green and geometry temporaries. */
    __asm__("" : "+r"(red) : "r"(input) : "$2", "$30");
    green=g;
    center_x=160;
    /* ASM_BARRIER: capture green and the X center before using V0 for geometry. */
    __asm__("" : "+r"(green), "+r"(center_x) : : "$2");
    temp=input>>1;
    /* ASM_BARRIER: preserve the intermediate before masking its low bit. */
    __asm__("" : "+r"(temp));
    interlace=temp&1;
    temp=interlace+1;
    height=temp<<8;
    center_y=height>>1;
    /* ASM_BARRIER: finish the Y center before reusing V0 for the opposite bank offset. */
    __asm__("" : "+r"(center_y) : : "$2");
    temp=1-interlace;
    other_y=temp<<8;
    SetGeomOffset(center_x,center_y);
    SetGeomScreen(290);
    width=320;
    rgb24=input&FRAME_RGB24;
    saved_flags=input;
    if(rgb24)width=480;
    env=DrawBuffers;
    SetDefDrawEnv(&env->buffer.environment00.draw00,0,0,width,height);
    SetDefDrawEnv(&DrawBuffers[1].buffer.environment00.draw00,0,other_y,width,height);
    SetDefDispEnv(&env->buffer.environment00.display5C,0,other_y,width,height);
    SetDefDispEnv(&DrawBuffers[1].buffer.environment00.display5C,0,0,width,height);
    i=0;
    background=saved_flags&FRAME_CLEAR_BACKGROUND;
    one=1;
    settings=Options.halves;
    loopRgb24=rgb24;
    window.y=0;window.x=0;window.h=0;window.w=0;
loop:
    {
        env->buffer.environment00.draw00.r=red;env->buffer.environment00.draw00.g=green;env->buffer.environment00.draw00.b=b;
        env->buffer.environment00.draw00.background=background;
        env->buffer.environment00.draw00.dither=one;
        env->buffer.environment00.draw00.window=window;
        env->buffer.environment00.display5C.screen.x=settings[0x34/2];
        env->buffer.environment00.display5C.screen.y=settings[0x38/2];
        env->buffer.environment00.display5C.display.h=height;
        env->buffer.environment00.display5C.flags10.bytes.interlace=interlace;
        env->buffer.environment00.display5C.screen.h=256;
        if(loopRgb24) {
            env->buffer.environment00.display5C.flags10.bytes.rgb24=one;
            env->buffer.environment00.display5C.display.w=(env->buffer.environment00.display5C.display.w*2)/3;
        }
        env++;
    }
    i++;
    if(i<2)goto loop;
    InitFrameEnvStub(0);
    InitFrameEnvStub(1);
}
